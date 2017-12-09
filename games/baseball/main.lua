local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  left  = { forms.vk_left,  'Left' },
  right = { forms.vk_right, 'Right' },
  a     = { forms.vk_right, 'Right' },
  l1    = { 49,             'Game A' },
  r1    = { 50,             'Game B' },
  l2    = { 51,             'Time' },
  r2    = { 53,             'Light' }
}

local menu = {
  { unit1.form1.btn_mode_down,   'Time',   51 },
  { unit1.form1.btn_game_a_down, 'Game A', 49 },
  { unit1.form1.btn_game_b_down, 'Game B', 50 },
  { unit1.form1.btn_light_down,  'Light',  53 }
}

local timers = {
  unit1.form1.timer_bonus,
  unit1.form1.timer_clock,
  unit1.form1.timer_date,
  unit1.form1.timer_game,
  unit1.form1.timer_game_over,
  unit1.form1.timer_game_reprise,
  unit1.form1.timer_game_start,
  unit1.form1.timer_got,
  unit1.form1.timer_startup
}

unit1.pfgameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfgot.data = system.loadbin( 'Got.pcm' )
unit1.pfmiss.data = system.loadbin( 'Miss.pcm' )
unit1.pfmove.data = system.loadbin( 'Move.pcm' )
unit1.pfstart.data = system.loadbin( 'Start.pcm' )

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
  { 200, 100, 159, 206 },
  menu
)
