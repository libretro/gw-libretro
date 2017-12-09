local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  up   = { forms.vk_left, forms.vk_up,    'Northwest' },
  down = { forms.vk_left, forms.vk_down,  'Southwest' },
  x    = { forms.vk_right, forms.vk_up,   'Northeast' },
  b    = { forms.vk_right, forms.vk_down, 'Southeast' },
  l1   = { 49,                            'Game A' },
  r1   = { 50,                            'Game B' },
  l2   = { 51,                            'Time' }
}

local menu = {
  { unit1.form1.btn_game_a_down, 'Game A', 49 },
  { unit1.form1.btn_game_b_down, 'Game B', 50 },
  { unit1.form1.btn_mode_down,   'Time',   51 }
}

local timers = {
  unit1.form1.timeralarm,
  unit1.form1.timerclock,
  unit1.form1.timerdate,
  unit1.form1.timerfork,
  unit1.form1.timergame,
  unit1.form1.timergameover,
  unit1.form1.timergamestart,
  unit1.form1.timermiss
}

unit1.pfgameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfgot.data = system.loadbin( 'Got.pcm' )
unit1.pfmiss.data = system.loadbin( 'Miss.pcm' )
unit1.pfstart.data = system.loadbin( 'Start.pcm' )
unit1.apfmove[ 1 ].data = system.loadbin( 'Moveup.pcm' )
unit1.apfmove[ 2 ].data = system.loadbin( 'Movedown.pcm' )

unit1.form1.oncreate()

-- this game doesn't have a acl mode so we have to reblit all images
unit1.form1.vid_showall( true )
unit1.form1.vid_showscore( 8888, true )

for i = 1, 4 do
  unit1.aim_panel_digits[ i ].visible = true
end

unit1.bsound = true

return system.init(
  unit1.form1.im_background,
  keymap,
  function( key ) unit1.form1.onkeydown( nil, key, 0 ) end,
  function( key ) unit1.form1.onkeyup( nil, key, 0 ) end,
  timers,
  { 153, 54, 257, 190 },
  menu
)
