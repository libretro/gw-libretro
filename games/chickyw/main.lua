local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  up    = { forms.vk_up,    'Up' },
  down  = { forms.vk_down,  'Down' },
  left  = { forms.vk_left,  'Left' },
  right = { forms.vk_right, 'Right' },
  l1    = { 0x31,           'Game A' },
  r1    = { 0x32,           'Game B' },
  l2    = { 0x33,           'Time' }
}

local menu = {
  { unit1.form1.btn_game_a_down, 'Game A', 0x31 },
  { unit1.form1.btn_game_b_down, 'Game B', 0x32 },
  { unit1.form1.btn_mode_down,   'Time',   0x33 }
}

local timers = {
  unit1.form1.timeralarm,
  unit1.form1.timerclock,
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
unit1.pfstart.data = system.loadbin( 'Start.pcm' )
unit1.pfstgstart.data = system.loadbin( 'Stgstart.pcm' )

-- Fix structures
do
  for i = 0, 6 do
    for j = 0, 8 do
      unit1.alabyrinth[ i ][ j ].chicky = {}
      unit1.alabyrinth[ i ][ j ].wolf = {}
    end
  end
end

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
  { 119, 31, 243, 211 },
  menu
)
