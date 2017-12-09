local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  up    = { forms.vk_up,      'Up' },
  down  = { forms.vk_down,    'Down' },
  left  = { forms.vk_left,    'Left' },
  right = { forms.vk_right,   'Right' },
  b     = { forms.vk_shift,   'Rotate' },
  a     = { forms.vk_control, 'Attack' },
  x     = { forms.vk_menu,    'Move' },
  l1    = { 53,               'ACL' }
}

local menu = {
  { unit1.form1.btn_acl_down, 'ACL', 53 }
}

local timers = {
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
}

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

unit1.form1.oncreate()

unit1.bsound = true
unit1.imode = 1
unit1.form1.gam_set_mode()

return system.init(
  unit1.form1.im_background,
  keymap,
  function( key ) unit1.form1.onkeydown( nil, key, 0 ) end,
  function( key ) unit1.form1.onkeyup( nil, key, 0 ) end,
  timers,
  { 73, 66, 207, 143 },
  menu
)
