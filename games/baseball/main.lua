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
unit1.imode = 0
unit1.form1.gam_set_mode()

local zoom = { left = 200, top = 100, width = 159, height = 206 }
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
    unit1.form1.timer_bonus,
    unit1.form1.timer_clock,
    unit1.form1.timer_date,
    unit1.form1.timer_game,
    unit1.form1.timer_game_over,
    unit1.form1.timer_game_reprise,
    unit1.form1.timer_game_start,
    unit1.form1.timer_got,
    unit1.form1.timer_startup
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
