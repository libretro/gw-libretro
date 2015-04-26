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

local function getpixel( png, x, y, transp )
  local r, g, b, a = image.split( png:getPixel( x, y ) )
  local c = rgbto16( r, g, b )
  
  if c == transp then
    r, g, b, a = 0, 0, 0, 0
  end
  
  if a >= 0 and a <= 31 then
    a = 0 -- transparent
  elseif a >= 32 and a <= 95 then
    a = 1 -- 25%
  elseif a >= 96 and a <= 159 then
    a = 2 -- 50%
  elseif a >= 160 and a <= 223 then
    a = 3 -- 75%
  else
    a = 4 -- opaque
  end
  
  return rgbto16( r, g, b ), a
end

local function rlerow( png, y, limit, transp )
  local rle = {}
  local width = png:getWidth()
  local used = 0
  
  local numcols = ( width + ( limit - 1 ) ) // limit
  
  for i = 1, numcols do
    rle[ #rle + 1 ] = 0
  end
  
  for xx = 0, width - 1, limit do
    rle[ xx // limit + 1 ] = #rle
    
    local x = xx
    rle[ #rle + 1 ] = 0
    local runs = #rle
    
    while x < ( xx + limit ) and x < width do
      local _, a = getpixel( png, x, y, transp )
      local count = 0
      local xsave = x
      
      while x < ( xx + limit ) and x < width do
        local c, aa = getpixel( png, x, y, transp )
        
        if aa ~= a then
          break
        end
        
        count = count + 1
        x = x + 1
      end
      
      rle[ #rle + 1 ] = ( a << 13 | count )
      rle[ runs ] = rle[ runs ] + 1
      
      if a ~= 0 then
        for i = 0, count - 1 do
          local c = getpixel( png, xsave + i, y, transp )
          rle[ #rle + 1 ] = c
          used = used + 1
        end
      end
    end
  end
  
  local final = newwriter()
  
  for i = 1, #rle do
    final:write16( rle[ i ] )
  end
  
  return final, used
end

local function rleimage( png, limit, transp )
  local width, height = png:getSize()
  local rows = {}
  local used = 0
  
  for y = 0, height - 1 do
    local row, u =  rlerow( png, y, limit, transp )
    rows[ y ] = row
    used = used + u
  end
  
  local rle = newwriter()
  
  rle:write16( width )
  rle:write16( height )
  rle:write32( used )
  
  local count = 0
  
  for y = 0, height - 1 do
    rle:write32( count )
    count = count + rows[ y ]:size()
  end
  
  for y = 0, height - 1 do
    rle:append( rows[ y ] )
  end
  
  return rle
end

return function( args )
  if #args == 0 then
    io.write( 'usage: luai rle.lua [ options ] <image>\n' )
    io.write( '--transp r g b   makes the given color transparent\n' )
    io.write( '--tl             makes the color at (0,0) transparent\n' )
    io.write( '--bl             makes the color at (0,height-1) transparent\n' )
    io.write( '--tr             makes the color at (width-1,0) transparent\n' )
    io.write( '--br             makes the color at (width-1,height-1) transparent\n' )
    io.write( '--margin x       sets the pixel limit on RLE runs on a row\n' )
    io.write( '                 (must be equal to RL_BACKGRND_MARGIN)\n' )
    
    return 0
  end
  
  local name, limit, transp
  
  for i = 1, #args do
    if args[ i ] == '--transp' then
      transp = rgbto16( tonumber( args[ i + 1 ] ), tonumber( args[ i + 2 ] ), tonumber( args[ i + 3 ] ) )
      i = i + 3
    elseif args[ i ] == '--tl' or args[ i ] == '--bl' or args[ i ] == '--tr' or args[ i ] == '--br' then
      transp = args[ i ]
    elseif args[ i ] == '--margin' then
      limit = tonumber( args[ i + 1 ] )
      i = i + 1
    else
      name = args[ i ]
    end
  end
  
  name = path.realpath( name )
  local png = image.load( name )
  
  if transp == '--tl' then
    transp = rgbto16( image.split( png:getPixel( 0, 0 ) ) )
  elseif transp == '--bl' then
    transp = rgbto16( image.split( png:getPixel( 0, png:getHeight() - 1 ) ) )
  elseif transp == '--tr' then
    transp = rgbto16( image.split( png:getPixel( png:getWidth() - 1, 0 ) ) )
  elseif transp == '--br' then
    transp = rgbto16( image.split( png:getPixel( png:getWidth() - 1, png:getHeight() - 1 ) ) )
  end
  
  local rle = rleimage( png, limit or 1e10 , transp )
  local dir, name, ext = path.split( name )
  rle:save( dir .. path.separator .. name .. '.rle' )
  return 0
end
