local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'
local controls = system.loadunit 'controls'

unit1.form1.oncreate()

unit1.pfarrow.data = system.loadbin( 'Arrow.pcm' )
unit1.pfbat.data = system.loadbin( 'Bat.pcm' )
unit1.pfcursor.data = system.loadbin( 'Cursor.pcm' )
unit1.pfdragon.data = system.loadbin( 'Dragon.pcm' )
unit1.pffound.data = system.loadbin( 'Found.pcm' )
unit1.pfgameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfgot.data = system.loadbin( 'Got.pcm' )
unit1.pfpit.data = system.loadbin( 'Pit.pcm' )
unit1.pfstep.data = system.loadbin( 'Step.pcm' )
unit1.pfstepbat.data = system.loadbin( 'Step_bat.pcm' )
unit1.pfwin.data = system.loadbin( 'Win.pcm' )

unit1.bsound = true
unit1.imode = 1
unit1.form1.gam_set_mode()

local zoom = { left = 73, top = 66, width = 207, height = 143 }
local cursor, right = system.splith( zoom )
local attack, move = system.splitv( right )

return system.init{
  background = unit1.form1.im_background,
  zoom = zoom,

  controls = {
    {
      button = unit1.form1.btn_acl_top,
      label  = 'ACL'
    },
    {
      button = unit1.form1.btn_cursor_top,
      zone   = cursor,
      label  = 'Cursor',
      lbl_dy = 32,
      keys   = { b = true },
      xkeys  = { forms.vk_shift }
    },
    {
      button = unit1.form1.btn_attack_top,
      zone   = attack,
      label  = 'Attack',
      keys   = { a = true },
      xkeys  = { forms.vk_control }
    },
    {
      button = unit1.form1.btn_move_top,
      zone   = move,
      label  = 'Move',
      keys   = { x = true },
      xkeys  = { forms.vk_menu }
    },
    {
      label  = 'Up',
      keys   = { up = true },
      xkeys  = { forms.vk_up }
    },
    {
      label  = 'Down',
      keys   = { down = true },
      xkeys  = { forms.vk_down }
    },
    {
      label  = 'Left',
      keys   = { left = true },
      xkeys  = { forms.vk_left }
    },
    {
      label  = 'Right',
      keys   = { right = true },
      xkeys  = { forms.vk_right }
    }
  },

  timers = {
    unit1.form1.timer_bat_moves,
    unit1.form1.timer_bat_warrior_move,
    unit1.form1.timer_blinking,
    unit1.form1.timer_demo_mode,
    unit1.form1.timer_found_arrow_hook,
    unit1.form1.timer_found_dragon,
    unit1.form1.timer_game_over_lose,
    unit1.form1.timer_game_over_win,
    unit1.form1.timer_game_start,
    unit1.form1.timer_move_to_arrow,
    unit1.form1.timer_move_to_bat,
    unit1.form1.timer_move_to_dragon,
    unit1.form1.timer_move_to_free,
    unit1.form1.timer_move_to_hook,
    unit1.form1.timer_move_to_pit,
    unit1.form1.timer_score,
    unit1.form1.timer_shoot_arrow
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
