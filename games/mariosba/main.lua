local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  left  = { forms.vk_left,    'Left' },
  right = { forms.vk_right,   'Right' },
  a     = { forms.vk_right,   'Right' },
  b     = { forms.vk_control, 'Bomb Up/Down' },
  l1    = { 49,               'Game A' },
  r1    = { 50,               'Game B' },
  l2    = { 51,               'Time' },
  r2    = { 53,               'ACL' }
}

local menu = {
  { unit1.form1.btn_game_a_down, 'Game A', 49 },
  { unit1.form1.btn_game_b_down, 'Game B', 50 },
  { unit1.form1.btn_time_down,   'Time',   51 },
  { unit1.form1.btn_acl_down,    'ACL',    53 }
}

local timers = {
  unit1.form1.timer_bonus_phase_1,
  unit1.form1.timer_bonus_phase_2,
  unit1.form1.timer_bonus_phase_3,
  unit1.form1.timer_bonus_phase_4,
  unit1.form1.timer_chance,
  unit1.form1.timer_clock,
  unit1.form1.timer_game,
  unit1.form1.timer_game_over,
  unit1.form1.timer_game_start,
  unit1.form1.timer_game_start_enemy,
  unit1.form1.timer_mario_move,
  unit1.form1.timer_miss_phase_1,
  unit1.form1.timer_miss_phase_2,
  unit1.form1.timer_miss_phase_3,
  unit1.form1.timer_score_blink,
  unit1.form1.timer_stage_init,
}

unit1.pfs_bombfall.data = system.loadbin( 'Bombfall.pcm' )
unit1.pfs_bonus0.data = system.loadbin( 'Bonus0.pcm' )
unit1.pfs_bonus1.data = system.loadbin( 'Bonus1.pcm' )
unit1.pfs_bonus2.data = system.loadbin( 'Bonus2.pcm' )
unit1.pfs_bonus3.data = system.loadbin( 'Bonus3.pcm' )
unit1.pfs_bonus4.data = system.loadbin( 'Bonus4.pcm' )
unit1.pfs_bonus5.data = system.loadbin( 'Bonus5.pcm' )
unit1.pfs_bonus6.data = system.loadbin( 'Bonus6.pcm' )
unit1.pfs_bonus7.data = system.loadbin( 'Bonus7.pcm' )
unit1.pfs_chance.data = system.loadbin( 'Chance.pcm' )
unit1.pfs_enemy.data = system.loadbin( 'Enemy.pcm' )
unit1.pfs_gameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfs_miss1.data = system.loadbin( 'Miss1.pcm' )
unit1.pfs_miss2.data = system.loadbin( 'Miss2.pcm' )
unit1.pfs_miss3.data = system.loadbin( 'Miss3.pcm' )
unit1.pfs_move.data = system.loadbin( 'Move.pcm' )
unit1.pfs_movebomb.data = system.loadbin( 'Movebomb.pcm' )
unit1.pfs_start.data = system.loadbin( 'Start.pcm' )

unit1.form1.oncreate()
unit1.form1.im_background_closed.visible = false
unit1.form1.im_background_closed = nil
unit1.form1.im_background_screen.layer = 0

unit1.bsound = true
unit1.imode = 2
unit1.form1.gam_set_mode()

return system.init(
  unit1.form1.im_background,
  keymap,
  function( key ) unit1.form1.onkeydown( nil, key, 0 ) end,
  function( key ) unit1.form1.onkeyup( nil, key, 0 ) end,
  timers,
  { 75, 107, 346, 186 },
  menu
)
