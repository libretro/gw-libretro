local image = require 'image'
local path = require 'path'

local function newwriter()
  return {
    bytes = {},
    write8 = function( self, x )
      self.bytes[ #self.bytes + 1 ] = string.char( x & 255 )
    end,
    write16 = function( self, x )
      self.bytes[ #self.bytes + 1 ] = string.char( ( x >> 8 ) & 255, x & 255 )
    end,
    write32 = function( self, x )
      self.bytes[ #self.bytes + 1 ] = string.char( ( x >> 24 ) & 255, ( x >> 16 ) & 255, ( x >> 8 ) & 255, x & 255 )
    end,
    append = function( self, rle )
      self.bytes[ #self.bytes + 1 ] = table.concat( rle.bytes )
    end,
    size = function( self )
      self.bytes = { table.concat( self.bytes ) }
      return #self.bytes[ 1 ]
    end,
    save = function( self, name )
      local file, err = io.open( name, 'wb' )
      if not file then error( err ) end
      self.bytes = { table.concat( self.bytes ) }
      file:write( self.bytes[ 1 ] )
      file:close()
    end
  }
end

local function rgbto16( r, g, b )
  r = r * 31 // 255
  g = g * 63 // 255
  b = b * 31 // 255
  
  return ( r << 11 ) | ( g << 5 ) | b
end

local function getpixel( png, x, y )
  local r, g, b = image.split( png:getPixel( x, y ) )
  return rgbto16( r, g, b )
end

local function mktileset( images )
  local writer = newwriter()
  
  writer:write16( images[ 1 ]:getWidth() )
  writer:write16( images[ 1 ]:getHeight() )
  writer:write16( #images )
  
  for _, img in pairs( images ) do
    for y = 0, img:getHeight() - 1 do
      for x = 0, img:getWidth() - 1 do
        writer:write16( getpixel( img, x, y ) )
      end
    end
  end
  
  return writer
end

local function main( args )
  if #args == 0 then
    io.write[[
rltileset.lua reads all images in the given directory and writes tileset data
that can be directly fed to rl_tileset_create. The first image it finds in the
given directory will dictate the width and height of all tiles in the tileset.
Subsequent images with different dimensions won't be written to the tileset.


Usage: luai rltileset.lua [ options ] <directory>

--output <file>  writes the tileset to the given file
                 (the default is <directory>.tls)

]]
    
    return 0
  end
  
  local dir, output
  
  for i = 1, #args do
    if args[ i ] == '--output' then
      output = args[ i + 1 ]
      i = i + 1
    else
      dir = args[ i ]
    end
  end
  
  dir = path.realpath( dir )
  local files = path.scandir( dir )
  local width, height
  local images = {}
  
  for _, file in ipairs( files ) do
    local stat = path.stat( file )
    
    if stat.file then
      local ok, png = pcall( image.load, file )
      
      if ok then
        if not width then
          width, height = png:getSize()
          images[ #images + 1 ] = png
          io.write( string.format( '%s set the tileset dimensions to %dx%d\n', file, width, height ) )
        elseif width == png:getWidth() and height == png:getHeight() then
          images[ #images + 1 ] = png
        else
          io.write( string.format( '%s doesn\'t have the required dimensions\n', file ) )
        end
      else
        io.write( string.format( '%s could not be read as an image\n', file ) )
      end
    end
  end
  
  if #images ~= 0 then
    local writer = mktileset( images )
    
    if not output then
      local dir, name, ext = path.split( dir )
      output = dir .. path.separator .. name .. '.tls'
    end
    
    writer:save( output )
  else
    io.write( 'no images were found\n' )
  end
  
  return 0
end

return main, mktileset
