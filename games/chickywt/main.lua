local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  up    = { forms.vk_up,    'Up' },
  down  = { forms.vk_down,  'Down' },
  left  = { forms.vk_left,  'Left' },
  right = { forms.vk_right, 'Right' },
  l1    = { 49,             'On/Off' },
  l2    = { 50,             'Select' },
  r1    = { 51,             'Start' }
}

local menu = {
  { unit1.form1.btn_on_down,     'On/Off', 49 },
  { unit1.form1.btn_select_down, 'Select', 50 },
  { unit1.form1.btn_start_down,  'Start',  51 }
}

local timers = {
  unit1.form1.timergame,
  unit1.form1.timergameover,
  unit1.form1.timergamestart,
  unit1.form1.timermiss,
  unit1.form1.timerpowerpill,
  unit1.form1.timerstagecomplete,
  unit1.form1.timerstagestart
}

unit1.pfbonus.data = system.loadbin( 'Bonus.pcm' )
unit1.pfcomplete.data = system.loadbin( 'Complete.pcm' )
unit1.pfgameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfmiss.data = system.loadbin( 'Miss.pcm' )
unit1.pfmove.data = system.loadbin( 'Move.pcm' )
unit1.pfpower1.data = system.loadbin( 'Power1.pcm' )
unit1.pfpower2.data = system.loadbin( 'Power2.pcm' )
unit1.pfpoweron.data = system.loadbin( 'Poweron.pcm' )
unit1.pfstart.data = system.loadbin( 'Start.pcm' )
unit1.pfstgstart.data = system.loadbin( 'Stgstart.pcm' )

unit1.form1.oncreate()
unit1.form1.im_hint_1.picture = nil
unit1.form1.im_hint_2.picture = nil
unit1.form1.im_hint_3.picture = nil

unit1.form1.im_background_screen.layer = 0;

unit1.bsound = true
unit1.imode = 0
unit1.form1.gam_setmode()

return system.init(
  unit1.form1.im_background,
  keymap,
  function( key ) unit1.form1.onkeydown( nil, key, 0 ) end,
  function( key ) unit1.form1.onkeyup( nil, key, 0 ) end,
  timers,
  { 239, 106, 195, 164 },
  menu
)
