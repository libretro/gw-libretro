local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  up    = { forms.vk_up,      'Up' },
  down  = { forms.vk_down,    'Down' },
  left  = { forms.vk_left,    'Left' },
  right = { forms.vk_right,   'Right' },
  b     = { forms.vk_control, 'Jump' },
  l1    = { 49,               'On/Off' },
  l2    = { 50,               'Select' },
  r1    = { 51,               'Start' }
}

local menu = {
  { unit1.form1.btn_power_down,  'On/Off', 49 },
  { unit1.form1.btn_select_down, 'Select', 50 },
  { unit1.form1.btn_start_down,  'Start',  51 }
}

local timers = {
  unit1.form1.timergame,
  unit1.form1.timergameover,
  unit1.form1.timergamestart,
  unit1.form1.timerhiscore,
  unit1.form1.timermiss,
  unit1.form1.timerstagecomplete,
}

unit1.pfbonus1.data = system.loadbin( 'Bonus1.pcm' )
unit1.pfbonus2.data = system.loadbin( 'Bonus2.pcm' )
unit1.pfgameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfjump.data = system.loadbin( 'Jump.pcm' )
unit1.pfmiss.data = system.loadbin( 'Miss.pcm' )
unit1.pfmove.data = system.loadbin( 'Move.pcm' )
unit1.pfmoveup.data = system.loadbin( 'Moveup.pcm' )
unit1.pfpoweron.data = system.loadbin( 'Poweron.pcm' )
unit1.pfpowerup.data = system.loadbin( 'Powerup.pcm' )
unit1.pfstart.data = system.loadbin( 'Start.pcm' )

unit1.form1.oncreate()
unit1.form1.im_hint_1.picture = nil
unit1.form1.im_hint_2.picture = nil
unit1.form1.im_hint_3.picture = nil

unit1.form1.im_background_screen.layer = 0;

unit1.bsound = true
unit1.imode = 0
unit1.form1.gam_set_mode()

return system.init(
  unit1.form1.im_background,
  keymap,
  function( key ) unit1.form1.onkeydown( nil, key, 0 ) end,
  function( key ) unit1.form1.onkeyup( nil, key, 0 ) end,
  timers,
  { 178, 108, 338, 239 },
  menu
)
