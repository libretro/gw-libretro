local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  up    = { forms.vk_up,      'Up' },
  down  = { forms.vk_down,    'Down' },
  left  = { forms.vk_left,    'Left' },
  right = { forms.vk_right,   'Right' },
  b     = { forms.vk_control, 'Fire' },
  l1    = { 49,               'On/Off' }
}

local menu = {
  { unit1.form1.btn_power_on, 'On/Off', 49 }
}

local timers = {
  unit1.form1.timer_game,
  unit1.form1.timer_game_over,
  unit1.form1.timer_reprise
}

unit1.pfs_gameover.data = system.loadbin( 'GameOver.pcm' )
unit1.pfs_hit.data = system.loadbin( 'Hit.pcm' )
unit1.pfs_missed.data = system.loadbin( 'Missed.pcm' )
unit1.pfs_sonar.data = system.loadbin( 'Sonar.pcm' )

unit1.form1.oncreate()

unit1.bsound = true
unit1.imode = 0
unit1.form1.gam_set_mode()

return system.init(
  unit1.form1.im_background,
  keymap,
  function( key ) unit1.form1.onkeydown( nil, key, 0 ) end,
  function( key ) unit1.form1.onkeyup( nil, key, 0 ) end,
  timers,
  { 107, 172, 277, 284 },
  menu
)
