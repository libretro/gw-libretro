return function( M )
  ----------------------------------------------------------------------------
  -- Setup version
  local major, minor, patch = M.GW_VERSIONSTR:match( '(%d+)%.(%d+)%.(%d+)' )
  
  M.GW_MAJOR = major + 0
  M.GW_MINOR = minor + 0
  M.GW_PATCH = patch + 0
  M.GW_VERSION = M.GW_MAJOR << 16 | M.GW_MINOR << 8 | M.GW_PATCH

  ----------------------------------------------------------------------------
  -- Define the function what loads Delphi units
  local cache = {}

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

  ----------------------------------------------------------------------------
  -- Redefine the log function to accept variable arguments
  local log = M.log

  M.log = function( ... )
    local msg = { ... }

    for i = 1, #msg do
      msg[ i ] = tostring( msg[ i ] )
    end

    log( table.concat( msg ), '' )
  end
  
  ----------------------------------------------------------------------------
  -- Create functions to divide rectangles
  M.splith = function( rect )
    local left  = { left = rect.left, top = rect.top, width = rect.width // 2, height = rect.height }
    local right = { left = rect.left + rect.width // 2, top = rect.top, width = rect.width // 2, height = rect.height }
    return left, right
  end

  M.splitv = function( rect )
    local top    = { left = rect.left, top = rect.top, width = rect.width, height = rect.height // 2 }
    local bottom = { left = rect.left, top = rect.top + rect.height // 2, width = rect.width, height = rect.height // 2 }
    return top, bottom
  end

  ----------------------------------------------------------------------------
  -- Create the TObject class
  local class = M.loadunit 'class'
  
  M.tobject = class.new()

  function M.tobject:new()
  end
  
  ----------------------------------------------------------------------------
  -- Load the controller and hand sprites
  local snes = M.newimage()
  snes.picture.data = M.loadbin( "snes.rle" )
  snes.visible = false
  
  local hand = M.newimage()
  hand.picture.data = M.loadbin( "hand.rle" )
  hand.visible = false

  ----------------------------------------------------------------------------
  -- Load the Boxy Bold font
  local boxybold = {}

  for char in string.gmatch( ' !"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`{|}~', '.' ) do
    local sprite = M.newimage()
    sprite.picture.data = M.loadbin( string.format( 'boxybold_%x.rle', char:byte() ) )
    sprite.visible = false

    boxybold[ char ] = sprite
  end
  
  for low in string.gmatch( 'abcdefghijklmnopqrstuvwxyz', '.' ) do
    boxybold[ low ] = boxybold[ low:upper() ]
  end
  
  ----------------------------------------------------------------------------
  -- Create a label
  local createlabel = function( x, y, text, alignment )
    local len = 1
    local sprites = {}
    
    for i = 1, #text do
      local sprite = boxybold[ text:sub( i, i ) ] or boxybold[ '?' ]
      sprites[ i ] = sprite.picture
      len = len + sprite.width - 1
    end
    
    if alignment == 'left' then
      -- nothing
    elseif alignment == 'right' then
      x = x - len
    else -- center
      x = x - len // 2
    end
    
    y = y - 8
    
    for i = 1, #sprites do
      local sprite = M.newimage()
      sprite.picture = sprites[ i ]
      sprites[ i ] = sprite
      
      sprite.left = x
      sprite.top = y
      sprite.visible = false
      sprite.layer = 30001
      
      x = x + sprite.width - 1
    end
    
    return {
      setvisible = function( self, visible )
        for i = 1, #sprites do
          sprites[ i ].visible = visible
        end
      end
    }
  end
  
  ----------------------------------------------------------------------------
  -- Create the controller help
  local createhelp = function( options, msgs )
    local positions = {
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
    
    local snes_x = ( options.background.width - snes.width ) // 2
    local snes_y = ( options.background.height - snes.height ) // 2
    
    local snes_spt = M.newimage()
    snes_spt.left = snes_x
    snes_spt.top = snes_y
    snes_spt.picture = snes.picture
    snes_spt.visible = false
    snes_spt.layer = 30000
    
    local labels = {}
    
    for _, control in ipairs( options.controls ) do
      if control.keys then
        for key in pairs( control.keys ) do
          local pos = positions[ key ]
          labels[ #labels + 1 ] = createlabel( snes_x + pos[ 1 ], snes_y + pos[ 2 ], control.label, pos[ 3 ] )
        end
      end
    end

    for key, msg in pairs( msgs ) do
      local pos = positions[ key ]
      labels[ #labels + 1 ] = createlabel( snes_x + pos[ 1 ], snes_y + pos[ 2 ], msg, pos[ 3 ] )
    end

    local setvisible = function( visible )
      snes_spt.visible = visible
      
      for _, label in ipairs( labels ) do
        label:setvisible( visible )
      end
    end

    return {
      show = function( self )
        setvisible( true )
      end,

      hide = function( self )
        setvisible( false )
      end
    }
  end

  ----------------------------------------------------------------------------
  -- Create the zones help
  local createzones = function( options )
    local labels = {}
    
    for _, control in ipairs( options.controls ) do
      local zone = control.zone

      if zone then
        local cx = zone.left + zone.width // 2
        local cy = zone.top + zone.height // 2

        cx = cx + ( control.lbl_dx or 0 )
        cy = cy + ( control.lbl_dy or 0 )

        labels[ #labels + 1 ] = createlabel( cx, cy, control.label )
      end
    end

    local zoom = options.zoom

    if zoom then
      local cx = zoom.left + zoom.width // 2
      local cy = zoom.top + zoom.height // 2

      labels[ #labels + 1 ] = createlabel( cx, cy - 8, 'Zoom' )
      labels[ #labels + 1 ] = createlabel( cx, cy + 8, 'Out' )
    end

    local setvisible = function( visible )
      for _, label in ipairs( labels ) do
        label:setvisible( visible )
      end
    end

    return {
      show = function( self )
        setvisible( true )
      end,

      hide = function( self )
        setvisible( false )
      end
    }
  end

  ----------------------------------------------------------------------------
  -- Create the hand menu
  local createmenu = function( options, help, zones )
    local menu = {}
    local entries = {}

    for _, control in ipairs( options.controls ) do
      if not control.xkeys then
        local button = control.button
        local btn_x, btn_y = button.left, button.top
        local btn_w, btn_h = button.width, button.height

        local center_x = btn_x + btn_w // 2
        local center_y = btn_y + btn_h // 2

        local hand_x, hand_y = center_x - 8, center_y

        local cursor = M.newimage()
        cursor.left, cursor.top = hand_x, hand_y
        cursor.picture = hand.picture
        cursor.visible = false
        cursor.layer = 30000

        local label = createlabel( hand_x + 14, hand_y + cursor.height + 10, control.label )

        local setvisible = function( visible )
          cursor.visible = visible
          label:setvisible( visible )
        end

        entries[ #entries + 1 ] = {
          show = function( self )
            setvisible( true )
          end,

          hide = function( self )
            setvisible( false )
          end,

          press = function( self )
            options.onbutton( button, true )
          end,

          release = function( self )
            options.onbutton( button, false )
          end
        }
      end
    end

    local zoom = options.zoom

    if zoom then
      local center_x = zoom.left + zoom.width // 2
      local center_y = zoom.top + zoom.height // 2

      if options.background.width < 480 then
        center_x = center_x - ( 480 - options.background.width ) // 2
      end

      local hand_x, hand_y = center_x - 8, center_y

      local cursor = M.newimage()
      cursor.left, cursor.top = hand_x, hand_y
      cursor.picture = hand.picture
      cursor.visible = false
      cursor.layer = 30000

      local zoom_in = createlabel( hand_x + 14, hand_y + cursor.height + 10, "Zoom" )
      local zoom_out = createlabel( hand_x + 14, hand_y + cursor.height + 10, "Cancel Zoom" )

      local setvisible = function( visible )
        cursor.visible = visible

        if menu.zoom then
          zoom_out:setvisible( visible )
        else
          zoom_in:setvisible( visible )
        end
      end

      entries[ #entries + 1 ] = {
        show = function( self )
          setvisible( true )
        end,

        hide = function( self )
          setvisible( false )
        end,

        press = function( self )
          menu.zoom = not menu.zoom
        end,

        release = function( self )
        end
      }
    end

    menu.state = 'inactive'
    menu.entries = entries
    menu.curr = 1
    menu.help = help
    menu.zones = zones
    menu.zoom = false

    menu.isactive = function( self )
      return self.state == 'active' or self.state == 'help'
    end

    menu.show = function( self )
      system.setzoom( nil )
      self:hidezones()
      self.state = 'active'
      self.curr = 1
      self.entries[ self.curr ]:show()
    end

    menu.hide = function( self )
      self.state = 'inactive'
      self:setzoom( self.zoom )
    end

    menu.showhelp = function( self )
      system.setzoom( nil )
      self:hidezones()
      self.state = 'help'
      self.help:show()
    end

    menu.hidehelp = function( self )
      self.state = 'inactive'
      self.help:hide()
      self:setzoom( self.zoom )
    end

    menu.showzones = function( self )
      self.zones:show()

      self.timer = system.newtimer()
      self.timer.interval = 3000
      self.timer.enabled = true
      self.timer.ontimer = function()
        self.zones:hide()
        self.timer.enabled = false
        self.timer = nil
      end
    end

    menu.hidezones = function( self )
      if self.timer then
        self.zones:hide()
        self.timer.enabled = false
        self.timer = nil
      end
    end

    menu.setzoom = function( self, zoom, showzones )
      self.zoom = zoom
      system.setzoom( self.zoom and options.zoom or nil )

      if not zoom then
        self:hidezones()
      end

      if zoom and showzones then
        self:showzones()
      end
    end

    menu.next = function( self )
      self.entries[ self.curr ]:hide()
      self.curr = self.curr + 1

      if self.curr <= #self.entries then
        self.entries[ self.curr ]:show()
      else
        self:hide()
      end
    end

    menu.press = function( self )
      self.entries[ self.curr ]:hide()
      self.entries[ self.curr ]:press()
      self.state = 'pressing'
    end

    menu.onkeydown = function( self, button )
      if self.state == 'active' then
        if button == 'select' then
          self:next()
        elseif button == 'start' then
          self:press()
        end
      elseif self.state == 'inactive' then
        if button == 'select' then
          self:show()
        elseif button == 'start' then
          self:showhelp()
        end
      elseif self.state == 'help' then
        if button == 'start' then
          self:hidehelp()
        elseif button == 'select' then
          self:hidehelp()
          self:show()
        end
      end
    end

    menu.onkeyup = function( self, button )
      if button == 'start' then
        if self.state == 'pressing' then
          self.entries[ self.curr ]:release()
          self:hide()
        end
      end
    end

    menu.tick = function( self )
      if self.timer then
        self.timer:tick()
      end
    end

    return menu
  end
  
  M.init = function( background, keymap, keydown, keyup, timers, zoom, options )
    if type( background ) ~= 'table' then
      -- Use the compatibility init function
      local compatinit = M.loadunit( 'compatinit' )
      M.snes = snes
      M.hand = hand
      M.render = createlabel
      return compatinit( background, keymap, keydown, keyup, timers, zoom, options, M )
    else
      options = background
      
      local zoom = options.zoom
      zoom[ 1 ] = zoom.left
      zoom[ 2 ] = zoom.top
      zoom[ 3 ] = zoom.width
      zoom[ 4 ] = zoom.height
    end

    local msgs = {
      select = 'Menu',
      start  = 'Select',
      cred1  = 'MADrigal\'s Simulators',
      cred2  = 'libretro port by @leiradel'
    }

    local help = createhelp( options, msgs )
    local zones = createzones( options )
    local menu = createmenu( options, help, zones )
    local state, newstate = {}, {}
    
    system.setbackground( options.background.picture )
    options.background.picture = nil

    return function()
      system.inputstate( newstate )

      if state.pointer_pressed ~= newstate.pointer_pressed then
        local pressed = newstate.pointer_pressed
        state.pointer_pressed = pressed

        -- Only process clicks if the menu is not active
        if not menu:isactive() then
          local x, y = newstate.pointer_x, newstate.pointer_y
          local handled = false

          if pressed then
            if system.iszoomed() then
              local zoom = options.zoom
              local cx = zoom.left + zoom.width // 2
              local cy = zoom.top + zoom.height // 2
              local dx, dy = x - cx, y - cy
              local xx, yy = dx * dx, dy * dy

              if xx + yy <= 2500 then
                -- If the center of the zoomed screen has been clicked, zoom out
                menu:setzoom( false )
                handled = true
              end
            else
              local zoom = options.zoom

              if zoom then
                local x0, y0 = zoom.left, zoom.top
                local x1, y1 = x0 + zoom.width, y0 + zoom.height

                if x >= x0 and x < x1 and y >= y0 and y < y1 then
                  -- If the screen has been clicked and zoom is supported, zoom in and show the zones
                  menu:setzoom( true, true )
                  handled = true
                end
              end
            end
          end

          if not handled then
            -- If the click has not already been handled, look for a control
            for i = 1, #options.controls do
              local control = options.controls[ i ]

              if system.iszoomed() then
                if control.zone  then
                  local zone = control.zone
                  local x0, y0 = zone.left, zone.top
                  local x1, y1 = x0 + zone.width, y0 + zone.height

                  if x >= x0 and x < x1 and y >= y0 and y < y1 then
                    -- If the screen is zoomed and a zone has been clicked, send the key events
                    for i = 1, #control.xkeys do
                      options.onkey( control.xkeys[ i ], pressed )
                    end

                    handled = true
                    break
                  end
                end
              else
                if control.button then
                  local button = control.button
                  local x0, y0 = button.left, button.top
                  local x1, y1 = x0 + button.width, y0 + button.height

                  if x >= x0 and x < x1 and y >= y0 and y < y1 then
                    -- If the screen is not zoomed and a button has been clicked, send key events or press the button
                    if control.xkeys then
                      for i = 1, #control.xkeys do
                        options.onkey( control.xkeys[ i ], pressed )
                      end
                    else
                      options.onbutton( button, pressed )
                    end

                    handled = true
                    break
                  end
                end
              end
            end
          end

          if not handled and options.rawmouse then
            options.rawmouse( x, y, pressed) 
          end
        end
      end

      -- Remove pointer events from the table
      newstate.pointer_x = nil
      newstate.pointer_y = nil
      newstate.pointer_pressed = nil
      
      for key, pressed in pairs( newstate ) do
        if state[ key ] == nil then
          state[ key ] = false
        end

        if state[ key ] ~= pressed then
          state[ key ] = pressed

          if not menu:isactive() then
            local handled = false

            for i = 1, #options.controls do
              local control = options.controls[ i ]

              if control.keys and control.keys[ key ] then
                -- If a control responds to the pressed key, send the key events
                for j = 1, #control.xkeys do
                  options.onkey( control.xkeys[ j ], pressed )
                end

                handled = true
              end
            end

            if not handled and key ~= 'select' and key ~= 'start' and options.rawkey then
              options.rawkey( key, pressed )
            end
          end
          
          -- Send the key to the menu
          if pressed then
            menu:onkeydown( key )
          else
            menu:onkeyup( key )
          end
        end
      end

      menu:tick()
      
      if not menu:isactive() then
        -- If the game is active, make sure sounds are playing, and tick the timers
        if not system.issoundactive() then
          system.resumesounds()
        end

        for _, timer in ipairs( options.timers ) do
          timer:tick()
        end
      else
        -- If the menu is active, pause the sounds
        if system.issoundactive() then
          system.pausesounds()
        end
      end

      -- Returns whether the game is active or not, to avoid advancing the game time
      return not menu:isactive()
    end
  end
end
