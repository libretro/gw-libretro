local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  left  = { forms.vk_left,  'Left' },
  right = { forms.vk_right, 'Right' },
  a     = { forms.vk_right, 'Right' },
  l1    = { 49,             'Mode' },
  r1    = { 50,             'Sound' },
  r2    = { 53,             'ACL' }
}

local menu = {
  { unit1.form1.btn_sound_down, 'Sound', 50 },
  { unit1.form1.btn_mode_down,  'Mode',  49 },
  { unit1.form1.btn_acl_down,   'ACL',   53 }
}

local timers = {
  unit1.form1.timer_clock,
  unit1.form1.timer_game_start,
  unit1.form1.timer_game_over,
  unit1.form1.timer_game,
  unit1.form1.timer_game_got,
  unit1.form1.timer_game_miss,
  unit1.form1.timer_slot_777_start,
  unit1.form1.timer_slot_777_action,
  unit1.form1.timer_slot_777_spin,
  unit1.form1.timer_slot_777_bonus1,
  unit1.form1.timer_slot_777_bonus2,
  unit1.form1.timer_slot_777_bonus3,
  unit1.form1.timer_slot_777_miss
}

unit1.pfs_coin.data = system.loadbin( 'Coin.pcm' )
unit1.pfs_gameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfs_got.data = system.loadbin( 'Got.pcm' )
unit1.pfs_miss.data = system.loadbin( 'Miss.pcm' )
unit1.pfs_start.data = system.loadbin( 'Start.pcm' )

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
  { 169, 86, 208, 140 },
  menu
)
