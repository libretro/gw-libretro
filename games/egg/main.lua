local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  up   = { forms.vk_left, forms.vk_up,    'Northwest' },
  down = { forms.vk_left, forms.vk_down,  'Southwest' },
  x    = { forms.vk_right, forms.vk_up,   'Northeast' },
  b    = { forms.vk_right, forms.vk_down, 'Southeast' },
  l1   = { 49,                            'Game A' },
  r1   = { 50,                            'Game B' },
  l2   = { 51,                            'Time' },
  r2   = { 53,                            'ACL' }
}

local menu = {
  { unit1.form1.btn_game_a_down, 'Game A', 49 },
  { unit1.form1.btn_game_b_down, 'Game B', 50 },
  { unit1.form1.btn_time_down,   'Time',   51 },
  { unit1.form1.btn_acl_down,    'ACL',    53 }
}

local actions = {
  { unit1.form1.btn_1_nw_down, 'up' },
  { unit1.form1.btn_3_sw_down, 'down' },
  { unit1.form1.btn_2_ne_down, 'x' },
  { unit1.form1.btn_4_se_down, 'b' }
}

local timers = {
  unit1.form1.timer_chance,
  unit1.form1.timer_clock,
  unit1.form1.timer_game,
  unit1.form1.timer_game_over,
  unit1.form1.timer_miss_long,
  unit1.form1.timer_miss_short,
  unit1.form1.timer_point,
  unit1.form1.timer_observer,
  unit1.form1.timer_show_miss
}

unit1.apfs_move[ 1 ].data = system.loadbin( 'Move_nw.pcm' )
unit1.apfs_move[ 2 ].data = system.loadbin( 'Move_ne.pcm' )
unit1.apfs_move[ 3 ].data = system.loadbin( 'Move_sw.pcm' )
unit1.apfs_move[ 4 ].data = system.loadbin( 'Move_se.pcm' )
unit1.pfs_gameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfs_miss.data = system.loadbin( 'Miss.pcm' )
unit1.pfs_point.data = system.loadbin( 'Point.pcm' )

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
  { 175, 105, 305, 189 },
  menu
)
