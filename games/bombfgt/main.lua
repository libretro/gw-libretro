local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'
local controls = system.loadunit 'controls'

unit1.form1.oncreate()

unit1.pf_enemy.data = system.loadbin( 'Enemy.pcm' )
unit1.pf_gameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pf_miss.data = system.loadbin( 'Miss.pcm' )
unit1.pf_shot.data = system.loadbin( 'Shot.pcm' )
unit1.pf_start.data = system.loadbin( 'Start.pcm' )

unit1.bsound = true
unit1.imode = 0
unit1.form1.gam_setmode()

local zoom = { left = 185, top = 50, width = 169, height = 206 }
local fire, bottom = system.splitv( zoom )
local left, right = system.splith( bottom )

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
      button = unit1.form1.btn_left_down,
      zone   = left,
      label  = 'Left',
      keys   = { left = true },
      xkeys  = { forms.vk_left }
    },
    {
      button = unit1.form1.btn_right_down,
      zone   = right,
      label  = 'Right',
      keys   = { right = true, a = true },
      xkeys  = { forms.vk_right }
    },
    {
      button = unit1.form1.btn_shoot_left_top,
      zone   = fire,
      label  = 'Shoot',
      keys   = { b = true },
      xkeys  = { forms.vk_control }
    },
    {
      button = unit1.form1.btn_shoot_right_top,
      label  = 'Shoot',
      keys   = { b = true },
      xkeys  = { forms.vk_menu }
    }
  },

  timers = {
    unit1.form1.timerclock,
    unit1.form1.timerdate,
    unit1.form1.timergame,
    unit1.form1.timergamestart,
    unit1.form1.timergameover,
    unit1.form1.timerintermission,
    unit1.form1.timermiss,
    unit1.form1.timershot,
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
