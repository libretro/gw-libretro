local image = require 'image'
local path = require 'path'

local function rgbto16( r, g, b )
  r = r * 31 // 255
  g = g * 63 // 255
  b = b * 31 // 255
  
  return ( r << 11 ) | ( g << 5 ) | b
end

return function( args )
  if #args == 0 then
    io.write[[
Creates a .h header file with the tile ready to be used in tile functions
rl_tile_blit_nobg, rl_tile_blit and rl_tile_unblit. The header file will have
a pixel array with the same name as the input image with extension, and two
macros with the tile's width and height will be defined.

Ex. "luai rltile.lua grass.png" will create a grass.h file containing the
grass_png pixel array and the grass_png_width and grass_png_height macros.

Usage: luai rltile.lua <image>

]]
    
    return 0
  end
  
  local name   = path.realpath( args[ 1 ] )
  local png    = image.load( name )
  local width  = png:getWidth()
  local height = png:getHeight()
  local pixels = {}
  
  for y = 0, height - 1 do
    for x = 0, width - 1 do
      local c = rgbto16( image.split( png:getPixel( x, y ) ) )
      pixels[ #pixels + 1 ] = c
    end
  end
  
  local dir, name, ext = path.split( name )
  local file, err = io.open( dir .. path.separator .. name .. '.h', 'w' )
  if not file then error( err ) end
  
  local array = string.gsub( name .. ext, '[%.\\/:]', '_' )
  
  file:write( 'const uint16_t ', array, '[] = {\n' )
  
  for i = 1, #pixels, 8 do
    local line = {}
    table.move( pixels, i, i + 7, 1, line )
    file:write( '  ' )
    
    for j = 1, #line do
      file:write( string.format( '0x%04x, ', line[ j ] ) )
    end
    
    file:write( '\n' )
  end
  
  file:write( '};\n\n' )
  file:write( '#define ', array, '_width ', tostring( width ), '\n' )
  file:write( '#define ', array, '_height ', tostring( height ), '\n' )
  file:close()
  
  return 0
end
