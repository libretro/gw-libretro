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

  local log = M.log

  M.log = function( ... )
    local msg = { ... }

    for i = 1, #msg do
      msg[ i ] = tostring( msg[ i ] )
    end

    log( table.concat( msg ), '' )
  end
  
  local class = M.loadunit 'class'
  
  M.tobject = class.new()

  function M.tobject:new()
  end
  
  local snes = M.newimage()
  snes.picture.data = M.loadbin( "snes.rle" )
  snes.visible = false
  
  local hand = M.newimage()
  hand.picture.data = M.loadbin( "hand.rle" )
  hand.visible = false
  
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
      spt.layer = 30001
      
      x = x + spt.width - 1
    end
    
    return {
      setvisible = function( self, visible )
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
      select = { 167, 152, 'right' },
      start  = { 232, 152 },
      cred1  = { 207, 208 },
      cred2  = { 207, 232 }
    }
    
    local snes_x = ( width - snes.width ) // 2
    local snes_y = ( height - snes.height ) // 2
    
    local snes_spt = M.newimage()
    snes_spt.left = snes_x
    snes_spt.top = snes_y
    snes_spt.picture = snes.picture
    snes_spt.visible = false
    snes_spt.layer = 30000
    
    local text = {}
    
    for button, pos in pairs( pos ) do
      text[ button ] = render( snes_x + pos[ 1 ], snes_y + pos[ 2 ], msgs[ button ] or '', pos[ 3 ] )
    end
    
    return {
      setvisible = function( self, visible )
        snes_spt.visible = visible
        
        for _, render in pairs( text ) do
          render:setvisible( visible )
        end
      end
    }
  end

  local createmenu = function( keydown, keyup, zoom, width, options, help )
    local entries = {}
    local menu = {}

    for _, entry in ipairs( options ) do
      local btn_x, btn_y = entry[ 1 ].left, entry[ 1 ].top
      local btn_w, btn_h = entry[ 1 ].width, entry[ 1 ].height

      local center_x = btn_x + btn_w // 2
      local center_y = btn_y + btn_h // 2

      local hand_x = center_x - 8
      local hand_y = center_y

      local cursor = M.newimage()
      cursor.left, cursor.top = hand_x, hand_y
      cursor.picture = hand.picture
      cursor.visible = false
      cursor.layer = 30000

      local text = render( hand_x + 14, hand_y + cursor.height + 10, entry[ 2 ] )

      entries[ #entries + 1 ] = {
        setvisible = function( self, visible )
          cursor.visible = visible
          text:setvisible( visible )
        end,

        activate = function( self )
          keydown( entry[ 3 ] )
        end,

        deactivate = function( self )
          keyup( entry[ 3 ] )
        end
      }
    end

    if zoom then
      local center_x = zoom[ 1 ] + zoom[ 3 ] // 2
      local center_y = zoom[ 2 ] + zoom[ 4 ] // 2

      if width < 480 then
        center_x = center_x - ( 480 - width ) // 2
      end

      local hand_x, hand_y = center_x - 8, center_y

      local cursor = M.newimage()
      cursor.left, cursor.top = hand_x, hand_y
      cursor.picture = hand.picture
      cursor.visible = false
      cursor.layer = 30000

      local text1 = render( hand_x + 14, hand_y + cursor.height + 10, "Zoom" )
      local text2 = render( hand_x + 14, hand_y + cursor.height + 10, "Cancel Zoom" )

      entries[ #entries + 1 ] = {
        setvisible = function( self, visible )
          cursor.visible = visible

          if menu.zoom then
            text2:setvisible( visible )
          else
            text1:setvisible( visible )
          end
        end,

        activate = function( self )
          menu.zoom = not menu.zoom
        end,

        deactivate = function( self )
        end
      }
    end

    menu.state = 'inactive'
    menu.entries = entries
    menu.curr = 1
    menu.help = help
    menu.zoom = false

    menu.isactive = function( self )
      return self.state == 'active' or self.state == 'help'
    end

    menu.menuon = function( self )
      system.setzoom( nil )
      self.state = 'active'
      self.curr = 1
      self.entries[ self.curr ]:setvisible( true )
    end

    menu.menuoff = function( self )
      self.state = 'inactive'
      system.setzoom( menu.zoom and zoom or nil)
    end

    menu.helpon = function( self )
      system.setzoom( nil )
      self.state = 'help'
      self.help:setvisible( true )
    end

    menu.helpoff = function( self )
      self.state = 'inactive'
      self.help:setvisible( false )
      system.setzoom( menu.zoom and zoom or nil)
    end

    menu.next = function( self )
      self.entries[ self.curr ]:setvisible( false )
      self.curr = self.curr + 1

      if self.curr > #self.entries then
        self:menuoff()
      else
        self.entries[ self.curr ]:setvisible( true )
      end
    end

    menu.choose = function( self )
      self.entries[ self.curr ]:setvisible( false )
      self.entries[ self.curr ]:activate()
      self.state = 'pressing'
    end

    menu.onkeydown = function( self, button )
      if self.state == 'active' then
        if button == 'select' then
          self:next()
        elseif button == 'start' then
          self:choose()
        end
      elseif self.state == 'inactive' then
        if button == 'select' then
          self:menuon()
        elseif button == 'start' then
          self:helpon()
        end
      elseif self.state == 'help' then
        if button == 'start' then
          self:helpoff()
        elseif button == 'select' then
          self:helpoff()
          self:menuon()
        end
      end
    end

    menu.onkeyup = function( self, button )
      if button == 'start' then
        if self.state == 'pressing' then
          self.entries[ self.curr ]:deactivate()
          self:menuoff()
        end
      end
    end

    return menu
  end
  
  M.init = function( background, keymap, keydown, keyup, timers, zoom, options )
    system.setbackground( background.picture )
    local bgwidth, bgheight = background.width, background.height
    background.picture = nil

    local state, newstate = {}, {}
    local zoomed = false

    if options then
      local del = {}

      for _, option in ipairs( options ) do
        for button, keys in pairs( keymap ) do
          if keys[ 1 ] == option[ 3 ] then
            del[ button ] = true
          end
        end
      end

      for button in pairs( del ) do
        keymap[ button ] = nil
      end
    end

    local msgs = {}
    
    for button, keys in pairs( keymap ) do
      msgs[ button ] = keys[ #keys ]
      keys[ #keys ] = nil
    end

    if options then
      msgs.select = 'Menu'
      msgs.start = 'Select'
    else
      msgs.select = zoom and 'Zoom' or ''
      msgs.start = 'Help'
    end

    msgs.cred1 = 'MADrigal\'s Simulators'
    msgs.cred2 = 'libretro port by @leiradel'

    local help = createhelp( bgwidth, bgheight, msgs )
    local menu

    if options then
      menu = createmenu( keydown, keyup, zoom, bgwidth, options, help )
    end
    
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
          
          if menu then
            if pressed then
              menu:onkeydown( button )
            else
              menu:onkeyup( button )
            end
          else
            if button == 'start' then
              if pressed then
                system.setzoom( nil )
              end

              help:setvisible( pressed )

              if not pressed then
                system.setzoom( zoomed and zoom or nil )
              end
            elseif button == 'select' and pressed then
              zoomed = not zoomed
              system.setzoom( zoomed and zoom or nil )
            end
          end
        end
      end
      
      if menu and not menu:isactive() then
        for _, timer in ipairs( timers ) do
          timer:tick()
        end
      end

      return state
    end
  end
end
