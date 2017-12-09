local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  up =   { forms.vk_left, forms.vk_up,    'Luigi Up' },
  down = { forms.vk_left, forms.vk_down,  'Luigi Down' },
  x =    { forms.vk_right, forms.vk_up,   'Mario Up' },
  b =    { forms.vk_right, forms.vk_down, 'Mario Down' },
  l1 =   { 49,                            'Game A' },
  r1 =   { 50,                            'Game B' },
  l2 =   { 51,                            'Time' },
  r2 =   { 53,                            'ACL' }
}

local menu = {
  { unit1.form1.btn_acl_down,    'ACL',    53 },
  { unit1.form1.btn_game_a_down, 'Game A', 49 },
  { unit1.form1.btn_game_b_down, 'Game B', 50 },
  { unit1.form1.btn_time_down,   'Time',   51 }
}

local timers = {  
  unit1.form1.timer_bonus,
  unit1.form1.timer_box_to_van,
  unit1.form1.timer_chance,
  unit1.form1.timer_clock,
  unit1.form1.timer_game,
  unit1.form1.timer_game_over,
  unit1.form1.timer_luigi_moved_box,
  unit1.form1.timer_luigi_moving_box,
  unit1.form1.timer_mario_moved_box,
  unit1.form1.timer_mario_moving_box,
  unit1.form1.timer_miss,
  unit1.form1.timer_move_luigi_up,
  unit1.form1.timer_move_luigi_down,
  unit1.form1.timer_move_mario_up,
  unit1.form1.timer_move_mario_down,
  unit1.form1.timer_score_blink,
  unit1.form1.timer_van_smoke
}

unit1.pfs_miss1.data = system.loadbin( 'Miss1.pcm' )
unit1.pfs_miss2.data = system.loadbin( 'Miss2.pcm' )
unit1.pfs_movebox.data = system.loadbin( 'Movebox.pcm' )
unit1.pfs_movechar.data = system.loadbin( 'Movechar.pcm' )
unit1.pfs_point.data = system.loadbin( 'Point.pcm' )

unit1.form1.oncreate()

unit1.form1.im_background_closed.picture = nil

unit1.bsound = true
unit1.imode = 2
unit1.form1.gam_set_mode()

return system.init(
  unit1.form1.im_background_open,
  keymap,
  function( key ) unit1.form1.onkeydown( nil, key, 0 ) end,
  function( key ) unit1.form1.onkeyup( nil, key, 0 ) end,
  timers,
  nil,
  menu
)
