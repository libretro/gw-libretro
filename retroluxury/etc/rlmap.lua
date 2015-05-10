local image = require 'image'
local path = require 'path'

local rleimage

local xml = [===[
local function prettyPrint( node, file, ident )
  file = file or io.stdout
  ident = ident or 0

  if type( node ) == 'table' then
    file:write( ( ' ' ):rep( ident ), '<', node.label )
    
    for attr, value in pairs( node.xarg ) do
      file:write( ' ', attr, '="', value, '"' )
    end
    
    if node.empty then
      file:write( '/>\n' )
    else
      file:write( '>\n' )
      
      for _, child in ipairs( node ) do
        prettyPrint( child, file, ident + 2 )
      end
      
      file:write( ( ' ' ):rep( ident ), '</', node.label, '>\n' )
    end
  else
    file:write( ( ' ' ):rep( ident ), node, '\n' )
  end
end

local function findNode( node, label )
  if type( node ) == 'table' then
    if node.label == label then
      return node
    end
    
    for i = 1, #node do
      local res = findNode( node[ i ], label )
      
      if res then
        return res
      end
    end
  end
end
  
local function parseargs(s)
  local arg = {}
  string.gsub(s, "([%w:]+)=([\"'])(.-)%2", function (w, _, a)
    arg[w] = a
  end)
  return arg
end

return
{
  parse = function(s)
    local stack = {}
    local top = {}
    table.insert(stack, top)
    local ni,c,label,xarg, empty
    local i, j = 1, 1
    while true do
      ni,j,c,label,xarg, empty = string.find(s, "<(%/?)([%w:]+)(.-)(%/?)>", i)
      if not ni then break end
      local text = string.sub(s, i, ni-1)
      if not string.find(text, "^%s*$") then
        table.insert(top, text)
      end
      if empty == "/" then  -- empty element tag
        table.insert(top, {label=label, xarg=parseargs(xarg), empty=1})
      elseif c == "" then   -- start tag
        top = {label=label, xarg=parseargs(xarg)}
        table.insert(stack, top)   -- new level
      else  -- end tag
        local toclose = table.remove(stack)  -- remove top
        top = stack[#stack]
        if #stack < 1 then
          error("nothing to close with "..label)
        end
        if toclose.label ~= label then
          error("trying to close "..toclose.label.." with "..label)
        end
        table.insert(top, toclose)
      end
      i = j+1
    end
    local text = string.sub(s, i)
    if not string.find(text, "^%s*$") then
      table.insert(stack[#stack], text)
    end
    if #stack > 1 then
      error("unclosed "..stack[#stack].label)
    end
    return stack[1][1]
  end,

  findNode = findNode,
  
  findAttr = function( node, name )
    for key, value in pairs( node.xarg ) do
      if key == name then
        return value
      end
    end
  end,
  
  prettyPrint = prettyPrint
}
]===]

xml = load( xml, 'xml.lua' )()

local function dump( t, i )
  i = i or 0
  local s = string.rep( ' ', i * 2 )
  
  if type( t ) == 'table' then
    io.write( s, '{\n' )
    for k, v in pairs( t ) do
      io.write( s, '  ', tostring( k ), ' = ' )
      dump( v, i + 1 )
    end
    io.write( s, '}\n' )
  elseif type( t ) == 'string' then
    io.write( s, string.format( '%q', t ), '\n' )
  else
    io.write( s, tostring( t ), '\n' )
  end
end

local function split( str, sep )
  sep = sep or ' '
  local res = {}
  local i = 1
  
  while #str ~= 0 do
    local j = str:find( sep, i, true )
    
    if not j then
      j = #str + 1
    end
    
    res[ #res + 1 ] = str:sub( i, j - 1 )
    str = str:sub( j + 1 )
  end
  
  return res
end

local function loadtmx( filename )
  local dir = path.split( filename ) .. path.separator
  local file, err = io.open( filename )
  
  if not file then
    error( err )
  end
  
  file:read( '*l' ) -- skip <?xml ... ?>
  local contents = file:read( '*a' )
  file:close()
  
  local tmx = xml.parse( contents )
  
  local map = {}
  
  map.version = xml.findAttr( tmx, 'version' )
  map.orientation = xml.findAttr( tmx, 'orientation' )
  map.width = tonumber( xml.findAttr( tmx, 'width' ) )
  map.height = tonumber( xml.findAttr( tmx, 'height' ) )
  map.tilewidth = tonumber( xml.findAttr( tmx, 'tilewidth' ) )
  map.tileheight = tonumber( xml.findAttr( tmx, 'tileheight' ) )
  map.widthpixels = map.width * map.tilewidth
  map.heightpixels = map.height * map.tileheight
  map.backgroundcolor = image.color( 0, 0, 0 )
  
  local backgroundcolor = xml.findAttr( tmx, 'backgroundcolor' )
  
  if backgroundcolor then
    backgroundcolor = tonumber( backgroundcolor, 16 )
    local r = backgroundcolor >> 16
    local g = backgroundcolor >> 8 & 255
    local b = backgroundcolor & 255
    map.backgroundcolor = image.color( r, g, b )
  end
  
  local tilesets = {}
  map.tilesets = tilesets
  
  local gids = {}
  map.gids = gids
  
  for _, child in ipairs( tmx ) do
    if child.label == 'tileset' then
      local tileset =
      {
        firstgid = tonumber( xml.findAttr( child, 'firstgid' ) ),
        name = xml.findAttr( child, 'name' ),
        tilewidth = tonumber( xml.findAttr( child, 'tilewidth' ) ),
        tileheight = tonumber( xml.findAttr( child, 'tileheight' ) )
      }
      
      if map.tilewidth ~= tileset.tilewidth or map.tileheight ~= tileset.tileheight then
        error( string.format( 'tile dimensions in %s are different from tile dimensions in map', tileset.name ) )
      end
      
      for _, child2 in ipairs( child ) do
        if child2.label == 'image' then
          local filename = path.realpath( dir .. xml.findAttr( child2, 'source' ) )
          tileset.image = image.load( filename )
          local trans = xml.findAttr( child2, 'trans' )
          
          if trans then
            trans = tonumber( trans, 16 )
            local r = trans >> 16
            local g = trans >> 8 & 255
            local b = trans & 255
            trans = image.color( r, g, b )
            tileset.image:colorToAlpha( trans )
          end
        end
      end
      
      tileset.lastgid = tileset.firstgid + ( tileset.image:getWidth() // tileset.tilewidth ) * ( tileset.image:getHeight() // tileset.tileheight ) - 1
      
      local imagewidth = tileset.image:getWidth()
      local tilewidth = tileset.tilewidth
      local tileheight = tileset.tileheight
      
      for i = tileset.firstgid, tileset.lastgid do
        local id = i - tileset.firstgid
        local j = id * tileset.tilewidth
        local x = j % imagewidth
        local y = math.floor( j / imagewidth ) * tileset.tileheight
        gids[ i ] =
        {
          tileset = tileset,
          id = id,
          x = x,
          y = y,
          width = tilewidth,
          height = tileheight,
          image = tileset.image:sub( x, y, x + tilewidth - 1, y + tileheight - 1 )
        }
      end
      
      tilesets[ #tilesets + 1 ] = tileset
    end
  end
  
  local layers = {}
  map.layers = layers
  
  for _, child in ipairs( tmx ) do
    if child.label == 'layer' then
      local layer =
      {
        name = xml.findAttr( child, 'name' ),
        width = tonumber( xml.findAttr( child, 'width' ) ),
        height = tonumber( xml.findAttr( child, 'height' ) ),
        tiles = {}
      }
      
      for _, child2 in ipairs( child ) do
        if child2.label == 'data' then
          local index = 1
          
          for y = 1, layer.height do
            local row = {}
            layer.tiles[ y ] = row
            
            for x = 1, layer.width do
              local tile = child2[ index ]
              index = index + 1
              row[ x ] = tonumber( xml.findAttr( tile, 'gid' ) )
            end
          end
        end
      end
      
      layers[ #layers + 1 ] = layer
    end
  end
  
  return map
end

local function render( map, layers )
  local png = image.create( map.widthpixels, map.heightpixels, image.color( 0, 0, 0, 0 ) )
  
  for _, layer in ipairs( map.layers ) do
    if layers[ layer.name ] then
      local yy = 0
      
      for y = 1, layer.height do
        local row = layer.tiles[ y ]
        local xx = 0
        
        for x = 1, layer.width do
          if row[ x ] ~= 0 then
            local tile = map.gids[ row[ x ] ]
            
            if not tile then
              error( 'Unknown gid ' .. row[ x ] .. ' in layer ' .. layer.name )
            end
            
            tile.image:blit( png, xx, yy )
          end
            
          xx = xx + map.tilewidth
        end
        
        yy = yy + map.tileheight
      end
    end
  end
  
  return png
end

local function newwriter()
  return {
    content = {},
    writeu8 = function( self, x )
      self.content[ #self.content + 1 ] = string.char( x & 255 )
    end,
    writeu16 = function( self, x )
      self.content[ #self.content + 1 ] = string.char( ( x >> 8 ) & 255, x & 255 )
    end,
    writeu32 = function( self, x )
      self.content[ #self.content + 1 ] = string.char( ( x >> 24 ) & 255, ( x >> 16 ) & 255, ( x >> 8 ) & 255, x & 255 )
    end,
    prependu32 = function( self, x )
      table.insert( self.content, 1, string.char( ( x >> 24 ) & 255, ( x >> 16 ) & 255, ( x >> 8 ) & 255, x & 255 ) )
    end,
    append = function( self, bytes )
      self.content[ #self.content + 1 ] = bytes
    end,
    save = function( self, filename )
      local file, err = io.open( filename, 'wb' )
      if not file then error( err ) end
      file:write( table.concat( self.content ) )
      file:close()
    end,
    size = function( self, filename )
      self.content = { table.concat( self.content ) }
      return #self.content[ 1 ]
    end,
    getcontent = function( self )
      self.content = { table.concat( self.content ) }
      return self.content[ 1 ]
    end
  }
end

local function compile( map, layers, coll )
  local built = { tiles = {}, images = {}, layer0 = {}, layers = {} }
  local out = newwriter()
  
  -- build layer 0 and the tileset
  do
    local names = {}
    
    for i = 1, #layers[ 1 ] do
      for _, layer in ipairs( map.layers ) do
        if layer.name == layers[ 1 ][ i ] then
          names[ layer.name ] = layer
        end
      end
    end
    
    local png = render( map, names )
    local tileset = {}
    
    for y = 0, png:getHeight() - 1, map.tileheight do
      built.layer0[ y // map.tileheight + 1 ] = {}
      
      for x = 0, png:getWidth() - 1, map.tilewidth do
        local sub = png:sub( x, y, x + 31, y + 31 )
        
        if not tileset[ sub:getHash() ] then
          built.tiles[ #built.tiles + 1 ] = sub
          tileset[ sub:getHash() ] = #built.tiles - 1
        end
        
        built.layer0[ y // map.tileheight + 1 ][ x // map.tilewidth + 1 ] = tileset[ sub:getHash() ]
      end
    end
  end
  
  -- build the other layers and the imageset
  do
    for l = 2, #layers do
      local names = {}
      
      for i = 1, #layers[ l ] do
        for _, layer in ipairs( map.layers ) do
          if layer.name == layers[ l ][ i ] then
            names[ layer.name ] = layer
          end
        end
      end
      
      local png = render( map, names )
      local imageset = {}
      
      local indices = {}
      built.layers[ l - 1 ] = indices
      
      for y = 0, png:getHeight() - 1, map.tileheight do
        indices[ y // map.tileheight + 1 ] = {}
        
        for x = 0, png:getWidth() - 1, map.tilewidth do
          local sub = png:sub( x, y, x + 31, y + 31 )
          
          if not sub:invisible() then
            if not imageset[ sub:getHash() ] then
              built.images[ #built.images + 1 ] = sub
              imageset[ sub:getHash() ] = #built.images
            end
            
            indices[ y // map.tileheight + 1 ][ x // map.tilewidth + 1 ] = imageset[ sub:getHash() ]
          else
            indices[ y // map.tileheight + 1 ][ x // map.tilewidth + 1 ] = 0
          end
        end
      end
    end
  end
  
  -- rl_map_t
  out:writeu16( map.width )
  out:writeu16( map.height )
  out:writeu16( 1 + #built.layers ) -- layer count
  
  -- rl_tileset_t
  out:writeu32( map.tilewidth * map.tileheight * 2 * #built.tiles + 6 ) -- total tileset size
  out:writeu16( map.tilewidth )
  out:writeu16( map.tileheight )
  out:writeu16( #built.tiles )
  
  for _, tile in ipairs( built.tiles ) do
    for y = 0, map.tileheight - 1 do
      for x = 0, map.tilewidth - 1 do
        local r, g, b = image.split( tile:getPixel( x, y ) )
        r, g, b = r * 31 // 255, g * 63 // 255, b * 31 // 255
        out:writeu16( ( r << 11 ) | ( g << 5 ) | b )
      end
    end
  end
  
  -- rl_imageset_t
  do
    local o = newwriter()
    o:writeu16( #built.images )
    
    for _, image in ipairs( built.images ) do
      local rle = rleimage( image, 32 ):get()
      o:writeu32( #rle )
      o:append( rle )
    end
    
    out:writeu32( o:size() )
    out:append( o:getcontent() )
  end
  
  -- rl_layer0
  for y = 1, map.height do
    for x = 1, map.width do
      out:writeu16( built.layer0[ y ][ x ] )
    end
  end
  
  -- rl_layern
  for _, layer in ipairs( built.layers ) do
    for y = 1, map.height do
      for x = 1, map.width do
        out:writeu16( layer[ y ][ x ] )
      end
    end
  end
  
  return out
end

return function( args )
  if #args < 2 then
    io.write[[
An utility to work with Tiled maps and convert them to retroluxury.

rlmap understands the following commands:

* list:    Lists the layers and/or tilesets in a map.

* render:  Renders the map as a PNG image. The command accepts a list of layer
           names and will render only them if the list is given.

* compile: Converts the map into a format ready to be used with rl_map_create.
           If a collision layer is given with --coll, all non-zero tiles
           represent unpassable tiles. The map resulting layers are a
           combination of one or more map layers, they can be compined with
           the + operator. Ex.: floor+flobjs fences+walls will create a map
           with two layers, the first with the combined floor and flobjs
           layers and the other with the fences and walls layers combined.

Usage: rlmap <mapname.tmx> command args...

Commands:

  list [--layers] [--tilesets]          lists the map\'s layers and/or tilesets

  render [layername...]                 renders the map as a PNG image

  compile --coll layermame              compiles the map as a .map file
          layername[+layername]...

]]

    return 0
  end
  
  do
    local dir, _, _ = path.split( path.realpath( args[ 0 ] ) )
    local ok
    ok, rleimage = loadfile( dir .. path.separator .. 'rlrle.lua' )()
  end
  
  local filename = path.realpath( args[ 1 ] )
  local map =  loadtmx( filename )
  
  if args[ 2 ] == 'list' then
    local layers = false
    local tilesets = false
    
    for i = 3, #args do
      if args[ i ] == '--layers' then
        layers = true
      elseif args[ i ] == '--tilesets' then
        tilesets = true
      else
        error( 'unknown argument to list: ' .. args[ i ] )
      end
    end
    
    if not layers and not tilesets then
      layers, tilesets = true, true
    end
    
    if layers then
      for i, layer in ipairs( map.layers ) do
        io.write( string.format( 'Layer %d: %s\n', i, layer.name ) )
      end
    end
    
    if tilesets then
      for i, tileset in ipairs( map.tilesets ) do
        io.write( string.format( 'Tileset %d: %s\n', i, tileset.name ) )
      end
    end
  elseif args[ 2 ] == 'render' then
    local layers = {}
    
    for i = 3, #args do
      layers[ args[ i ] ] = true
    end
    
    if not next( layers ) then
      for i, layer in ipairs( map.layers ) do
        layers[ layer.name ] = true
      end
    end
    
    local dir, name, ext = path.split( filename )
    render( map, layers ):save( dir .. path.separator .. name .. '.png' )
  elseif args[ 2 ] == 'compile' then
    local layers = {}
    local coll = nil
    local ii = 3
    
    if args[ 3 ] == '--coll' then
      coll = split( args[ 4 ], '+' )
      ii = 5
    end
    
    for i = ii, #args do
      layers[ i - ii + 1 ] = split( args[ i ], '+' )
    end
    
    if #layers == 0 then
      error( 'the built map must have at least one layer' )
    end
    
    local out = compile( map, layers, coll )
    local dir, name, ext = path.split( filename )
    out:save( dir .. path.separator .. name .. '.map' )
  else
    error( 'unknown command: ' .. args[ 2 ] )
  end
end
