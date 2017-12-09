local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  up    = { forms.vk_up,      'Up' },
  down  = { forms.vk_down,    'Down' },
  left  = { forms.vk_left,    'Left' },
  right = { forms.vk_right,   'Right' },
  b     = { forms.vk_control, 'Jump' },
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
  unit1.form1.timer_chance,
  unit1.form1.timer_clock,
  unit1.form1.timer_coconut,
  unit1.form1.timer_coconut_bonus,
  unit1.form1.timer_coconut_music,
  unit1.form1.timer_flying_music,
  unit1.form1.timer_game,
  unit1.form1.timer_game_over,
  unit1.form1.timer_game_start,
  unit1.form1.timer_junior_1_1_hand,
  unit1.form1.timer_junior_6_hand,
  unit1.form1.timer_junior_jump,
  unit1.form1.timer_junior_step,
  unit1.form1.timer_key_blink,
  unit1.form1.timer_key_fall,
  unit1.form1.timer_kong_arm,
  unit1.form1.timer_miss_crow,
  unit1.form1.timer_miss_fall,
  unit1.form1.timer_score_blink,
  unit1.form1.timer_umbrella_balloon
}

unit1.pfs_bonus1.data = system.loadbin( 'Bonus1.pcm' )
unit1.pfs_bonus2.data = system.loadbin( 'Bonus2.pcm' )
unit1.pfs_bonus3.data = system.loadbin( 'Bonus3.pcm' )
unit1.pfs_chance.data = system.loadbin( 'Chance.pcm' )
unit1.pfs_coconut.data = system.loadbin( 'Coconut.pcm' )
unit1.pfs_crow.data = system.loadbin( 'Crow.pcm' )
unit1.pfs_falling.data = system.loadbin( 'Falling.pcm' )
unit1.pfs_flying.data = system.loadbin( 'Flying.pcm' )
unit1.pfs_hand1.data = system.loadbin( 'Hand1.pcm' )
unit1.pfs_hand2.data = system.loadbin( 'Hand2.pcm' )
unit1.pfs_jump.data = system.loadbin( 'Jump.pcm' )
unit1.pfs_key1.data = system.loadbin( 'Key1.pcm' )
unit1.pfs_key2.data = system.loadbin( 'Key2.pcm' )
unit1.pfs_miss.data = system.loadbin( 'Miss.pcm' )
unit1.pfs_point.data = system.loadbin( 'Point.pcm' )
unit1.pfs_start1.data = system.loadbin( 'Start1.pcm' )
unit1.pfs_start2.data = system.loadbin( 'Start2.pcm' )
unit1.pfs_step.data = system.loadbin( 'Step.pcm' )

unit1.form1.oncreate()

unit1.form1.im_background_screen.layer = 0;

unit1.bsound = true
unit1.imode = 1
unit1.form1.gam_set_mode()

return system.init(
  unit1.form1.im_background,
  keymap,
  function( key ) unit1.form1.onkeydown( nil, key, 0 ) end,
  function( key ) unit1.form1.onkeyup( nil, key, 0 ) end,
  timers,
  { 81, 96, 320, 205 },
  menu
)
