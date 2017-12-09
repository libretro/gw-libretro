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
  unit1.form1.timer_bonus_star,
  unit1.form1.timer_demostart,
  unit1.form1.timer_game,
  unit1.form1.timer_ghost,
  unit1.form1.timer_miss,
  unit1.form1.timer_stage_complete
}

unit1.pfbonus.data = system.loadbin( 'Bonus.pcm' )
unit1.pfenergiz.data = system.loadbin( 'Energiz.pcm' )
unit1.pfghost.data = system.loadbin( 'Ghost.pcm' )
unit1.pfmiss.data = system.loadbin( 'Miss.pcm' )
unit1.pfpill.data = system.loadbin( 'Pill.pcm' )
unit1.pfpill_alt.data = system.loadbin( 'Pill_alt.pcm' )
unit1.pfstage.data = system.loadbin( 'Stage.pcm' )
unit1.pfstart.data = system.loadbin( 'Start.pcm' )

unit1.form1.oncreate()
unit1.form1.im_hint_1.picture = nil
unit1.form1.im_hint_2.picture = nil
unit1.form1.im_hint_3.picture = nil

unit1.form1.im_background_screen.layer = 0;

unit1.bsound = true
unit1.imode = 0
unit1.form1.gam_set_game_mode()

return system.init(
  unit1.form1.im_background,
  keymap,
  function( key ) unit1.form1.onkeydown( nil, key, 0 ) end,
  function( key ) unit1.form1.onkeyup( nil, key, 0 ) end,
  timers,
  { 234, 114, 221, 205 },
  menu
)
