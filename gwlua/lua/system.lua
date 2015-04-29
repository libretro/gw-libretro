local cache = {}

return function( M )
  M.loadunit = function( name )
    local entry = name .. '.lua'
    local unit = cache[ entry ]
    
    if unit then
      return unit
    end

    local bin = M.loadbin( entry )

    if not bin then
      bin = M.loadbs( name .. '.bs' )
    end
    
    if bin then
      local err
      unit, err = load( bin, entry, 't' )
      
      if not unit then
        error( err )
      end
      
      unit = unit()
      cache[ entry ] = unit
      return unit
    end
    
    error( 'unit ' .. entry .. ' not found' )
  end
  
  local class = M.loadunit 'class'
  
  M.tobject = class.new()

  function M.tobject:new()
  end
  
  local snes = M.newimage()
  snes.picture.data = M.loadbin( "snes.rle" )
  snes.visible = false
  
  local sprites = {
    [ " " ] = "boxybold_20.rle",
    [ "!" ] = "boxybold_21.rle",
    [ "\"" ] = "boxybold_22.rle",
    [ "#" ] = "boxybold_23.rle",
    [ "$" ] = "boxybold_24.rle",
    [ "%" ] = "boxybold_25.rle",
    [ "&" ] = "boxybold_26.rle",
    [ "'" ] = "boxybold_27.rle",
    [ "(" ] = "boxybold_28.rle",
    [ ")" ] = "boxybold_29.rle",
    [ "*" ] = "boxybold_2a.rle",
    [ "+" ] = "boxybold_2b.rle",
    [ "," ] = "boxybold_2c.rle",
    [ "-" ] = "boxybold_2d.rle",
    [ "." ] = "boxybold_2e.rle",
    [ "/" ] = "boxybold_2f.rle",
    [ "0" ] = "boxybold_30.rle",
    [ "1" ] = "boxybold_31.rle",
    [ "2" ] = "boxybold_32.rle",
    [ "3" ] = "boxybold_33.rle",
    [ "4" ] = "boxybold_34.rle",
    [ "5" ] = "boxybold_35.rle",
    [ "6" ] = "boxybold_36.rle",
    [ "7" ] = "boxybold_37.rle",
    [ "8" ] = "boxybold_38.rle",
    [ "9" ] = "boxybold_39.rle",
    [ ":" ] = "boxybold_3a.rle",
    [ ";" ] = "boxybold_3b.rle",
    [ "<" ] = "boxybold_3c.rle",
    [ "=" ] = "boxybold_3d.rle",
    [ ">" ] = "boxybold_3e.rle",
    [ "?" ] = "boxybold_3f.rle",
    [ "@" ] = "boxybold_40.rle",
    [ "A" ] = "boxybold_41.rle",
    [ "B" ] = "boxybold_42.rle",
    [ "C" ] = "boxybold_43.rle",
    [ "D" ] = "boxybold_44.rle",
    [ "E" ] = "boxybold_45.rle",
    [ "F" ] = "boxybold_46.rle",
    [ "G" ] = "boxybold_47.rle",
    [ "H" ] = "boxybold_48.rle",
    [ "I" ] = "boxybold_49.rle",
    [ "J" ] = "boxybold_4a.rle",
    [ "K" ] = "boxybold_4b.rle",
    [ "L" ] = "boxybold_4c.rle",
    [ "M" ] = "boxybold_4d.rle",
    [ "N" ] = "boxybold_4e.rle",
    [ "O" ] = "boxybold_4f.rle",
    [ "P" ] = "boxybold_50.rle",
    [ "Q" ] = "boxybold_51.rle",
    [ "R" ] = "boxybold_52.rle",
    [ "S" ] = "boxybold_53.rle",
    [ "T" ] = "boxybold_54.rle",
    [ "U" ] = "boxybold_55.rle",
    [ "V" ] = "boxybold_56.rle",
    [ "W" ] = "boxybold_57.rle",
    [ "X" ] = "boxybold_58.rle",
    [ "Y" ] = "boxybold_59.rle",
    [ "Z" ] = "boxybold_5a.rle",
    [ "[" ] = "boxybold_5b.rle",
    [ "\\" ] = "boxybold_5c.rle",
    [ "]" ] = "boxybold_5d.rle",
    [ "^" ] = "boxybold_5e.rle",
    [ "_" ] = "boxybold_5f.rle",
    [ "`" ] = "boxybold_60.rle",
    [ "{" ] = "boxybold_7b.rle",
    [ "|" ] = "boxybold_7c.rle",
    [ "}" ] = "boxybold_7d.rle",
    [ "~" ] = "boxybold_7e.rle",
  }
  
  for char, name in pairs( sprites ) do
    local spt = M.newimage()
    spt.picture.data = M.loadbin( name )
    spt.visible = false
    sprites[ char ] = spt
  end
  
  sprites[ "a" ] = sprites.A
  sprites[ "b" ] = sprites.B
  sprites[ "c" ] = sprites.C
  sprites[ "d" ] = sprites.D
  sprites[ "e" ] = sprites.E
  sprites[ "f" ] = sprites.F
  sprites[ "g" ] = sprites.G
  sprites[ "h" ] = sprites.H
  sprites[ "i" ] = sprites.I
  sprites[ "j" ] = sprites.J
  sprites[ "k" ] = sprites.K
  sprites[ "l" ] = sprites.L
  sprites[ "m" ] = sprites.M
  sprites[ "n" ] = sprites.N
  sprites[ "o" ] = sprites.O
  sprites[ "p" ] = sprites.P
  sprites[ "q" ] = sprites.Q
  sprites[ "r" ] = sprites.R
  sprites[ "s" ] = sprites.S
  sprites[ "t" ] = sprites.T
  sprites[ "u" ] = sprites.U
  sprites[ "v" ] = sprites.V
  sprites[ "w" ] = sprites.W
  sprites[ "x" ] = sprites.X
  sprites[ "y" ] = sprites.Y
  sprites[ "z" ] = sprites.Z
  
  local render = function( x, y, text, alignment )
    local len = 1
    local list = {}
    
    for i = 1, #text do
      local spt = sprites[ text:sub( i, i ) ] or sprites[ '?' ]
      list[ i ] = spt.picture
      len = len + spt.width - 1
    end
    
    if alignment == 'left' then
      -- nothing
    elseif alignment == 'right' then
      x = x - len
    else -- center
      x = x - len // 2
    end
    
    y = y - 8
    
    for i = 1, #list do
      local spt = M.newimage()
      spt.picture = list[ i ]
      list[ i ] = spt
      
      spt.left = x
      spt.top = y
      spt.visible = false
      spt.layer = 200
      
      x = x + spt.width - 1
    end
    
    return {
      setVisible = function( self, visible )
        for i = 1, #list do
          list[ i ].visible = visible
        end
      end
    }
  end
  
  local createhelp = function( width, height, msgs )
    local pos = {
      up     = {  80,  72 },
      down   = {  80, 118 },
      left   = {  58,  95, 'right' },
      right  = { 102,  95, 'left' },
      a      = { 368,  96, 'left' },
      b      = { 334, 132 },
      x      = { 328,  59 },
      y      = { 294,  92, 'right' },
      l1     = {  76,   6 },
      r1     = { 338,   6 },
      l2     = {  12,  36 },
      r2     = { 402,  36 },
      select = { 167, 144 },
      start  = { 232, 144 }
    }
    
    local snes_x = ( width - snes.width ) // 2
    local snes_y = ( height - snes.height ) // 2
    
    local snes_spt = M.newimage()
    snes_spt.left = snes_x
    snes_spt.top = snes_y
    snes_spt.picture = snes.picture
    snes_spt.visible = false
    snes_spt.layer = 199
    
    local text = {}
    
    for button, pos in pairs( pos ) do
      text[ button ] = render( snes_x + pos[ 1 ], snes_y + pos[ 2 ], msgs[ button ] or '', pos[ 3 ] )
    end
    
    return {
      --timer = M.newtimer(),
      setVisible = function( self, visible )
        snes_spt.visible = visible
        
        for _, render in pairs( text ) do
          render:setVisible( visible )
        end
      end,
    }
  end
  
  M.init = function( background, keymap, keydown, keyup, timers, zoom )
    local msgs = {}
    
    for button, keys in pairs( keymap ) do
      msgs[ button ] = keys[ #keys ]
      keys[ #keys ] = nil
    end
    
    msgs.select = zoom and 'Zoom' or ''
    msgs.start = 'Help'
    
    system.setbackground( background.picture )
    local help = createhelp( background.width, background.height, msgs )
    background.picture = nil
    
    local state, newstate = {}, {}
    local zoomed = false
    
    return function()
      system.inputstate( newstate )
      
      for button, pressed in pairs( newstate ) do
        if state[ button ] ~= pressed then
          local keys = keymap[ button ]
          
          if keys then
            if pressed then
              for _, key in ipairs( keys ) do
                keydown( key )
              end
            else
              for _, key in ipairs( keys ) do
                keyup( key )
              end
            end
          end
          
          state[ button ] = pressed
          
          if button == 'start' then
            help:setVisible( state.start )
          end
          
          if button == 'select' and pressed then
            zoomed = not zoomed
            system.setzoom( zoomed and zoom or nil )
          end
        end
      end
      
      for _, timer in ipairs( timers ) do
        timer:tick()
      end
      
      return state
    end
  end
end
