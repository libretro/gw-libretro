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
  { unit1.form1.btn_acl_down,    'ACL',    53 },
  { unit1.form1.btn_game_a_down, 'Game A', 49 },
  { unit1.form1.btn_game_b_down, 'Game B', 50 },
  { unit1.form1.btn_time_down,   'Time',   51 }
}

local timers = {
  unit1.form1.timer_chance,
  unit1.form1.timer_clock,
  unit1.form1.timer_game,
  unit1.form1.timer_game_over,
  unit1.form1.timer_lifesaver,
  unit1.form1.timer_man_safe_left,
  unit1.form1.timer_man_safe_right,
  unit1.form1.timer_miss_1,
  unit1.form1.timer_miss_2,
  unit1.form1.timer_miss_3,
  unit1.form1.timer_score_blink,
  unit1.form1.timer_smoke
}

unit1.pfs_chance.data = system.loadbin( 'Chance.pcm' )
unit1.pfs_miss1.data = system.loadbin( 'Miss1.pcm' )
unit1.pfs_miss2.data = system.loadbin( 'Miss2.pcm' )
unit1.apfmove[ 2 ].data = system.loadbin( 'Move2.pcm' )
unit1.apfmove[ 3 ].data = system.loadbin( 'Move3.pcm' )
unit1.apfmove[ 4 ].data = system.loadbin( 'Move4.pcm' )
unit1.apfmove[ 5 ].data = system.loadbin( 'Move5.pcm' )

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
