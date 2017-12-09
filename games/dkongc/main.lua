local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  up    = { forms.vk_up,      'Up' },
  down  = { forms.vk_down,    'Down' },
  left  = { forms.vk_left,    'Left' },
  right = { forms.vk_right,   'Right' },
  b     = { forms.vk_control, 'Jump' },
  l1    = { 49,               'Power' }
}

local menu = {
  { unit1.form1.btn_power_down, 'Power', 49 }
}

local timers = {
  unit1.form1.timer_game_1,
  unit1.form1.timer_game_2,
  unit1.form1.timer_game_3,
  unit1.form1.timer_game_start,
  unit1.form1.timer_mario_move,
  unit1.form1.timer_miss,
  unit1.form1.timer_stage_complete,
  unit1.form1.timer_time_counter,
  unit1.form1.timer_woman_blink
}

unit1.pfbonus.data = system.loadbin( 'Bonus.pcm' )
unit1.pfbutton.data = system.loadbin( 'Button.pcm' )
unit1.pfcomplete.data = system.loadbin( 'Complete.pcm' )
unit1.pfgameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfjump.data = system.loadbin( 'Jump.pcm' )
unit1.pfhurryup.data = system.loadbin( 'Hurryup.pcm' )
unit1.pfmiss.data = system.loadbin( 'Miss.pcm' )
unit1.pfpower.data = system.loadbin( 'Power.pcm' )
unit1.pfstart1.data = system.loadbin( 'Start1.pcm' )
unit1.pfstart2.data = system.loadbin( 'Start2.pcm' )
unit1.pfstep1.data = system.loadbin( 'Step1.pcm' )
unit1.pfstep2.data = system.loadbin( 'Step2.pcm' )

unit1.form1.oncreate()
unit1.form1.im_background_on.layer = 0;
unit1.bsound = true

return system.init(
  unit1.form1.im_background,
  keymap,
  function( key ) unit1.form1.onkeydown( nil, key, 0 ) end,
  function( key ) unit1.form1.onkeyup( nil, key, 0 ) end,
  timers,
  { 178, 177, 137, 245 },
  menu
)
