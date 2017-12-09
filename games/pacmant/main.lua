local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  up    = { forms.vk_up,    'Up' },
  down  = { forms.vk_down,  'Down' },
  left  = { forms.vk_left,  'Left' },
  right = { forms.vk_right, 'Right' },
  r1    = { 50,             'On/Off' },
  l1    = { 49,             'Ama/Pro' },
}

local menu = {
  { unit1.form1.btn_pro_down,   'Ama/Pro',    49 },
  { unit1.form1.btn_power_down, 'On/Off', 50 }
}

local timers = {
  unit1.form1.timer_cherry_bonus,
  unit1.form1.timer_game,
  unit1.form1.timer_game_over,
  unit1.form1.timer_game_start,
  unit1.form1.timer_ghost_bonus,
  unit1.form1.timer_intermission,
  unit1.form1.timer_miss,
  unit1.form1.timer_power_pill_sound,
  unit1.form1.timer_stage_complete,
}

unit1.pfs_beep.data = system.loadbin( 'Beep.pcm' )
unit1.pfs_bonus.data = system.loadbin( 'Bonus.pcm' )
unit1.pfs_miss.data = system.loadbin( 'Miss.pcm' )
unit1.pfs_point.data = system.loadbin( 'Point.pcm' )
unit1.pfs_start.data = system.loadbin( 'Start.pcm' )

unit1.form1.oncreate()
unit1.form1.im_background_screen_off_top.layer = 0;
unit1.form1.im_background_screen_on.layer = 0;

unit1.bsound = true
unit1.imode = 0
unit1.form1.gam_set_mode()

return system.init(
  unit1.form1.im_background,
  keymap,
  function( key ) unit1.form1.onkeydown( nil, key, 0 ) end,
  function( key ) unit1.form1.onkeyup( nil, key, 0 ) end,
  timers,
  { 302, 218, 340, 130 },
  menu
)
