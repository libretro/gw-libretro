local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  up    = { forms.vk_up,      'Up' },
  down  = { forms.vk_down,    'Down' },
  left  = { forms.vk_left,    'Left' },
  right = { forms.vk_right,   'Right' },
  l1    = { 49,               'Amateur' },
  r1    = { 50,               'Professional' },
  l2    = { 51,               'Time' },
  r2    = { 53,               'Time/Game' }
}

local menu = {
  { unit1.form1.btn_mode_down, 'Time/Game',    53 },
  { unit1.form1.btn_ama_down,  'Amateur',      49 },
  { unit1.form1.btn_pro_down,  'Professional', 50 },
  { unit1.form1.btn_time_down, 'Time',         51 }
}

local timers = {
  unit1.form1.timer_cherry_bonus,
  unit1.form1.timer_clock,
  unit1.form1.timer_game,
  unit1.form1.timer_game_start,
  unit1.form1.timer_ghost_bonus,
  unit1.form1.timer_intermission,
  unit1.form1.timer_miss,
  unit1.form1.timer_power_pill_sound,
  unit1.form1.timer_stage_complete
}

unit1.pfs_beep.data = system.loadbin( 'Beep.pcm' )
unit1.pfs_bonus.data = system.loadbin( 'Bonus.pcm' )
unit1.pfs_miss.data = system.loadbin( 'Miss.pcm' )
unit1.pfs_point.data = system.loadbin( 'Point.pcm' )
unit1.pfs_start.data = system.loadbin( 'Start.pcm' )

unit1.form1.oncreate()

unit1.bsound = true

unit1.imode = 1
unit1.form1.gam_set_mode()

return system.init(
  unit1.form1.im_background,
  keymap,
  function( key ) unit1.form1.onkeydown( nil, key, 0 ) end,
  function( key ) unit1.form1.onkeyup( nil, key, 0 ) end,
  timers,
  { 75, 180, 327, 125 },
  menu
)
