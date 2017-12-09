local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  up   = { forms.vk_up  ,    'Up' },
  down = { forms.vk_down,    'Down' },
  b    = { forms.vk_control, 'Hit' },
  l1   = { 49,               'Game A' },
  r1   = { 50,               'Game B' },
  l2   = { 51,               'Time' },
  r2   = { 53,               'ACL' }
}

local menu = {
  { unit1.form1.btn_game_a_down, 'Game A', 49 },
  { unit1.form1.btn_game_b_down, 'Game B', 50 },
  { unit1.form1.btn_time_down,   'Time',   51 },
  { unit1.form1.btn_acl_down,    'ACL',    53 }
}

local timers = {
  unit1.form1.timer_chance,
  unit1.form1.timer_clock,
  unit1.form1.timer_game,
  unit1.form1.timer_game_first_ball,
  unit1.form1.timer_game_over,
  unit1.form1.timer_miss,
  unit1.form1.timer_miss_sounds,
  unit1.form1.timer_racket,
  unit1.form1.timer_score_blink,
  unit1.form1.timer_snore
}

unit1.pfs_back.data = system.loadbin( 'Back.pcm' )
unit1.pfs_ball.data = system.loadbin( 'Ball.pcm' )
unit1.pfs_chance.data = system.loadbin( 'Chance.pcm' )
unit1.pfs_gameover.data = system.loadbin( 'GameOver.pcm' )
unit1.pfs_hit.data = system.loadbin( 'Hit.pcm' )
unit1.pfs_miss.data = system.loadbin( 'Miss.pcm' )

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
  { 173, 100, 304, 193 },
  menu
)
