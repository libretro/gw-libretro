local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  left  = { forms.vk_left,    'Left' },
  right = { forms.vk_right,   'Right' },
  a     = { forms.vk_right,   'Right' },
  b     = { forms.vk_control, 'Shoot' },
  l1    = { 0x31,             'Game A' },
  r1    = { 0x32,             'Game B' },
  l2    = { 0x33,             'Time' }
}

local menu = {
  { unit1.form1.btn_mode_down,   'Time',   0x33 },
  { unit1.form1.btn_game_a_down, 'Game A', 0x31 },
  { unit1.form1.btn_game_b_down, 'Game B', 0x32 }
}

local timers = {
  unit1.form1.timerclock,
  unit1.form1.timerdate,
  unit1.form1.timergame,
  unit1.form1.timergamestart,
  unit1.form1.timergameover,
  unit1.form1.timerintermission,
  unit1.form1.timermiss,
  unit1.form1.timershot,
  unit1.form1.timerstartup
}

unit1.pf_enemy.data = system.loadbin( 'Enemy.pcm' )
unit1.pf_gameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pf_miss.data = system.loadbin( 'Miss.pcm' )
unit1.pf_shot.data = system.loadbin( 'Shot.pcm' )
unit1.pf_start.data = system.loadbin( 'Start.pcm' )

unit1.form1.oncreate()

unit1.bsound = true
unit1.imode = 0
unit1.form1.gam_setmode()

return system.init(
  unit1.form1.im_background,
  keymap,
  function( key ) unit1.form1.onkeydown( nil, key, 0 ) end,
  function( key ) unit1.form1.onkeyup( nil, key, 0 ) end,
  timers,
  { 188, 49, 166, 210 },
  menu
)
