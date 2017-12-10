local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'
local controls = system.loadunit 'controls'

unit1.form1.oncreate()

unit1.pfs_got.data = system.loadbin( 'Got.pcm' )
unit1.pfs_move.data = system.loadbin( 'Move.pcm' )
unit1.pfs_start.data = system.loadbin( 'Start.pcm' )

unit1.bsound = true
unit1.imode = 1
unit1.form1.gam_set_mode()

local zoom = { left = 176, top = 69, width = 260, height = 184 }
local left, right = system.splith( zoom )

return system.init{
  background = unit1.form1.im_background,
  zoom = zoom,

  controls = {
    {
      button = unit1.form1.btn_time_top,
      label  = 'Time'
    },
    {
      button = unit1.form1.btn_acl_top,
      label  = 'ACL'
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
    }
  },

  timers = {
    unit1.form1.timer_clock,
    unit1.form1.timer_game,
    unit1.form1.timer_game_over,
    unit1.form1.timer_game_reprise_hit,
    unit1.form1.timer_game_start,
    unit1.form1.timer_hero_animation,
    unit1.form1.timer_miss_enemy,
    unit1.form1.timer_miss_item
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
