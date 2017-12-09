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
  { unit1.form1.btn_time_down,   'Time',   51 },
  { unit1.form1.btn_game_a_down, 'Game A', 49 },
  { unit1.form1.btn_game_b_down, 'Game B', 50 },
  { unit1.form1.btn_acl_down,    'ACL',    53 }
}

local timers = {
  unit1.form1.timer_acl,
  unit1.form1.timer_clock_1,
  unit1.form1.timer_clock_2,
  unit1.form1.timer_game,
  unit1.form1.timer_game_start,
  unit1.form1.timer_miss,
  unit1.form1.timer_rescue_dropdown,
  unit1.form1.timer_rescue_pickup
}

unit1.pfs_bar.data = system.loadbin( 'Bar.pcm' )
unit1.pfs_danger.data = system.loadbin( 'Danger.pcm' )
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
  { 148, 67, 264, 177 },
  menu
)
