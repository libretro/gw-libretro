local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  up    = { forms.vk_up,      'Up' },
  down  = { forms.vk_down,    'Down' },
  left  = { forms.vk_left,    'Left' },
  right = { forms.vk_right,   'Right' },
  b     = { forms.vk_control, 'Speed/Shoot' },
  l1    = { 49,               'On/Off' },
}

local menu = {
  { unit1.form1.btn_power_down, 'On/Off', 49 }
}

local timers = {
  unit1.form1.timer_phase_1_2_3_mcp,
  unit1.form1.timer_phase_1_start,
  unit1.form1.timer_phase_1_cycle_tron,
  unit1.form1.timer_phase_1_cycle_sark,
  unit1.form1.timer_phase_1_win,
  unit1.form1.timer_phase_1_miss,
  unit1.form1.timer_phase_2_start,
  unit1.form1.timer_phase_2_tron_move,
  unit1.form1.timer_phase_2_sark_move,
  unit1.form1.timer_phase_2_sark_throw,
  unit1.form1.timer_phase_2_tron_disc,
  unit1.form1.timer_phase_2_sark_disc,
  unit1.form1.timer_phase_2_win,
  unit1.form1.timer_phase_2_miss_disc,
  unit1.form1.timer_phase_2_miss_life,
  unit1.form1.timer_phase_3_start,
  unit1.form1.timer_phase_3_clock,
  unit1.form1.timer_phase_3_tron_disc,
  unit1.form1.timer_phase_3_barrier_open,
  unit1.form1.timer_phase_3_barrier_close,
  unit1.form1.timer_phase_3_barrier_move,
  unit1.form1.timer_phase_3_win,
  unit1.form1.timer_phase_3_miss,
  unit1.form1.timer_game_over,
}

unit1.pfs_beep.data = system.loadbin( 'Beep.pcm' )
unit1.pfs_complete.data = system.loadbin( 'Complete.pcm' )
unit1.pfs_disc.data = system.loadbin( 'Disc.pcm' )
unit1.pfs_enginehi.data = system.loadbin( 'Enginehi.pcm' )
unit1.pfs_enginelo.data = system.loadbin( 'Enginelo.pcm' )
unit1.pfs_gameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfs_miss1.data = system.loadbin( 'Miss1.pcm' )
unit1.pfs_miss2.data = system.loadbin( 'Miss2.pcm' )
unit1.pfs_start.data = system.loadbin( 'Start.pcm' )
unit1.pfs_turn.data = system.loadbin( 'Turn.pcm' )
unit1.pfs_win.data = system.loadbin( 'Win.pcm' )

if system.GW_VERSION and system.GW_VERSION >= 0x010400 then
  unit1.pfs_enginehi.loop = true
  unit1.pfs_enginelo.loop = true
end

unit1.form1.oncreate()
unit1.form1.im_background_screen.layer = 0;

unit1.bsound = true
unit1.imode = 0

return system.init(
  unit1.form1.im_background,
  keymap,
  function( key ) unit1.form1.onkeydown( nil, key, 0 ) end,
  function( key ) unit1.form1.onkeyup( nil, key, 0 ) end,
  timers,
  { 209, 132, 348, 144 },
  menu
)
