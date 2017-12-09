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
  unit1.form1.timer_chance_time,
  unit1.form1.timer_clock,
  unit1.form1.timer_game,
  unit1.form1.timer_game_over,
  unit1.form1.timer_game_start,
  unit1.form1.timer_hero_hand_1,
  unit1.form1.timer_hero_hand_2,
  unit1.form1.timer_hero_hand_3,
  unit1.form1.timer_hero_hand_4,
  unit1.form1.timer_miss_1,
  unit1.form1.timer_miss_2,
  unit1.form1.timer_miss_3,
  unit1.form1.timer_score_blink
}

unit1.pfs_beep1.data = system.loadbin( 'Beep1.pcm' )
unit1.pfs_beep2.data = system.loadbin( 'Beep2.pcm' )
unit1.pfs_beep3.data = system.loadbin( 'Beep3.pcm' )
unit1.pfs_beep4.data = system.loadbin( 'Beep4.pcm' )
unit1.pfs_chance.data = system.loadbin( 'Chance.pcm' )
unit1.pfs_miss1.data = system.loadbin( 'Miss1.pcm' )
unit1.pfs_miss2.data = system.loadbin( 'Miss2.pcm' )
unit1.pfs_miss3.data = system.loadbin( 'Miss3.pcm' )
unit1.pfs_gameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfs_start.data = system.loadbin( 'Start.pcm' )
unit1.pfs_step.data = system.loadbin( 'Step.pcm' )

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
  { 79, 102, 332, 186 },
  menu
)
