local createhelp = function( width, height, msgs, M )
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
  
  local snes_x = ( width - M.snes.width ) // 2
  local snes_y = ( height - M.snes.height ) // 2
  
  local snes_spt = M.newimage()
  snes_spt.left = snes_x
  snes_spt.top = snes_y
  snes_spt.picture = M.snes.picture
  snes_spt.visible = false
  snes_spt.layer = 30000
  
  local text = {} for k,v in pairs(M) do print(k,v) end
  
  for button, pos in pairs( pos ) do
    text[ button ] = M.render( snes_x + pos[ 1 ], snes_y + pos[ 2 ], msgs[ button ] or '', pos[ 3 ] )
  end

  local help = { active = false }

  help.isactive = function( self )
    return self.active
  end
  
  help.setvisible = function( self, visible )
    snes_spt.visible = visible
    
    for _, render in pairs( text ) do
      render:setvisible( visible )
    end

    self.active = visible
  end
  
  return help
end

local createmenu = function( keydown, keyup, zoom, width, options, help, M )
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
    cursor.picture = M.hand.picture
    cursor.visible = false
    cursor.layer = 30000

    local text = M.render( hand_x + 14, hand_y + cursor.height + 10, entry[ 2 ] )

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
    cursor.picture = M.hand.picture
    cursor.visible = false
    cursor.layer = 30000

    local text1 = M.render( hand_x + 14, hand_y + cursor.height + 10, "Zoom" )
    local text2 = M.render( hand_x + 14, hand_y + cursor.height + 10, "Cancel Zoom" )

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

return function( background, keymap, keydown, keyup, timers, zoom, options, M )
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

  local help = createhelp( bgwidth, bgheight, msgs, M )
  local menu

  if options then
    menu = createmenu( keydown, keyup, zoom, bgwidth, options, help, M )
  end
  
  return function()
    local gameactive = true
    
    if help:isactive() then
      gameactive = false
    end
    
    if menu and menu:isactive() then
      gameactive = false
    end

    system.inputstate( newstate )
    
    for button, pressed in pairs( newstate ) do
      if state[ button ] ~= pressed then
        local keys = keymap[ button ]
        
        if keys then
          if gameactive then
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
    
    if gameactive then
      if not system.issoundactive() then
        system.resumesounds()
      end

      for _, timer in ipairs( timers ) do
        timer:tick()
      end
    else
      if system.issoundactive() then
        system.pausesounds()
      end
    end

    return gameactive
  end
end