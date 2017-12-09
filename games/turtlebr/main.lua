local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  left  = { forms.vk_left,  'Left' },
  right = { forms.vk_right, 'Right' },
  a     = { forms.vk_right, 'Right' },
  l1    = { 49,             'Game A' },
  r1    = { 50,             'Game B' },
  l2    = { 51,             'Time' },
  r2    = { 53,             'ACL' }
}

local menu = {
  { unit1.form1.btn_game_a_down, 'Game A', 49 },
  { unit1.form1.btn_game_b_down, 'Game B', 50 },
  { unit1.form1.btn_time_down,   'Time',   51 },
  { unit1.form1.btn_acl_down,    'ACL',    53 }
}

local timers = {
  unit1.form1.timer_bonus_left,
  unit1.form1.timer_bonus_right,
  unit1.form1.timer_chance_time,
  unit1.form1.timer_chance_time_blink,
  unit1.form1.timer_clock,
  unit1.form1.timer_game,
  unit1.form1.timer_game_over,
  unit1.form1.timer_hero_down,
  unit1.form1.timer_hero_jump,
  unit1.form1.timer_miss,
  unit1.form1.timer_tourist_right,
  unit1.form1.timer_turtle_head_1,
  unit1.form1.timer_turtle_head_2,
  unit1.form1.timer_turtle_head_3,
  unit1.form1.timer_turtle_head_4,
  unit1.form1.timer_turtle_head_5
}

unit1.pfs_chance.data = system.loadbin( 'Chance.pcm' )
unit1.pfs_fish.data = system.loadbin( 'Fish.pcm' )
unit1.pfs_gameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfs_miss.data = system.loadbin( 'Miss.pcm' )
unit1.pfs_point.data = system.loadbin( 'Point.pcm' )
unit1.pfs_step.data = system.loadbin( 'Step.pcm' )

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
  { 177, 102, 305, 195 },
  menu
)
