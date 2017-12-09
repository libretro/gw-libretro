local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  up   = { forms.vk_left, forms.vk_up,    'Northwest' },
  down = { forms.vk_left, forms.vk_down,  'Southwest' },
  x    = { forms.vk_right, forms.vk_up,   'Northeast' },
  b    = { forms.vk_right, forms.vk_down, 'Southeast' },
  l1    = { 49,                           'Game A' },
  r1    = { 50,                           'Game B' },
  l2    = { 52,                           'Game A+B' },
  r2    = { 53,                           'ACL' },
  y     = { 51,                           'Time' }
}

local menu = {
  { unit1.form1.btn_game_a,       'Game A',   49 },
  { unit1.form1.btn_game_b,       'Game B',   50 },
  { unit1.form1.btn_time,         'Time',     51 },
  { unit1.form1.btn_game_ab_down, 'Game A+B', 52 },
  { unit1.form1.btn_acl,          'ACL',      53 }
}

local timers = {
  unit1.form1.timerabpoint,
  unit1.form1.timerabmiss,
  unit1.form1.timerabshoot,
  unit1.form1.timerabshowmiss,
  unit1.form1.timerchance,
  unit1.form1.timerclock,
  unit1.form1.timerenemy,
  unit1.form1.timergameover,
  unit1.form1.timerpoint,
  unit1.form1.timermisslong,
  unit1.form1.timermissshort,
  unit1.form1.timership,
  unit1.form1.timershowmiss
}

unit1.apfbeeps[ 1 ].data = system.loadbin( 'Move_Nw.pcm' )
unit1.apfbeeps[ 2 ].data = system.loadbin( 'Move_Ne.pcm' )
unit1.apfbeeps[ 3 ].data = system.loadbin( 'Move_Sw.pcm' )
unit1.apfbeeps[ 4 ].data = system.loadbin( 'Move_Se.pcm' )
unit1.pfgameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfmiss.data = system.loadbin( 'Miss.pcm' )
unit1.pfpoint.data = system.loadbin( 'Point.pcm' )

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
