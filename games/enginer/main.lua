local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'
local controls = system.loadunit 'controls'

unit1.form1.oncreate()

unit1.pfgameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfgot.data = system.loadbin( 'Got.pcm' )
unit1.pfmiss.data = system.loadbin( 'Miss.pcm' )
unit1.pfmove.data = system.loadbin( 'Move.pcm' )
unit1.pfstart.data = system.loadbin( 'Start.pcm' )

unit1.bsound = true

-- this game doesn't have a acl mode so we have to reblit all images
do
  unit1.form1.vid_showall( true )
end

--unit1.imode = 0
--unit1.form1.gam_setmode()

local zoom = { left = 200, top = 99, width = 159, height = 207 }
local left, right = system.splith( zoom )

return system.init{
  background = unit1.form1.im_background,
  zoom = zoom,

  controls = {
    {
      button = unit1.form1.btn_mode_top,
      label  = 'Time'
    },
    {
      button = unit1.form1.btn_game_a_top,
      label  = 'Game A'
    },
    {
      button = unit1.form1.btn_game_b_top,
      label  = 'Game B'
    },
    {
      button = unit1.form1.btn_light_top,
      label  = 'Light'
    },
    {
      button = unit1.form1.btn_left_down,
      zone   = left,
      label  = 'Left',
      lbl_dy = 32,
      keys   = { left = true },
      xkeys  = { forms.vk_left }
    },
    {
      button = unit1.form1.btn_right_down,
      zone   = right,
      label  = 'Right',
      lbl_dy = 32,
      keys   = { right = true, a = true },
      xkeys  = { forms.vk_right }
    }
  },

  timers = {
    unit1.form1.timerbonus,
    unit1.form1.timerclock,
    unit1.form1.timerdate,
    unit1.form1.timergame,
    unit1.form1.timergameover,
    unit1.form1.timergamestart,
    unit1.form1.timergot,
    unit1.form1.timermiss,
    unit1.form1.timerstartup
  },

  onkey = function( key, pressed )
    local handler = pressed and unit1.form1.onkeydown or unit1.form1.onkeyup
    handler( nil, key, 0 )
  end,

  onbutton = function( button, pressed )
    local handler = pressed and button.onmousedown or button.onmouseup
    handler( nil, controls.mbleft, false, 0, 0 )
  end
}
