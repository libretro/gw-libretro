local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  left  = { forms.vk_left,  'Left' },
  right = { forms.vk_right, 'Right' },
  a     = { forms.vk_right, 'Right' },
  l1    = { 51,             'Time' },
  r1    = { 53,             'ACL' }
}

local menu = {
  { unit1.form1.btn_time_down, 'Time', 51 },
  { unit1.form1.btn_acl_down,  'ACL',  53 }
}

local timers = {
  unit1.form1.timer_clock,
  unit1.form1.timer_game,
  unit1.form1.timer_game_over,
  unit1.form1.timer_game_reprise_hit,
  unit1.form1.timer_game_start,
  unit1.form1.timer_hero_animation,
  unit1.form1.timer_miss_enemy,
  unit1.form1.timer_miss_item
}

unit1.pfs_got.data = system.loadbin( 'Got.pcm' )
unit1.pfs_move.data = system.loadbin( 'Move.pcm' )
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
  { 176, 69, 260, 184 },
  menu
)
