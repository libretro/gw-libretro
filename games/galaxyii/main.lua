local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  left  = { forms.vk_left,    'Left' },
  right = { forms.vk_right,   'Right' },
  b     = { forms.vk_control, 'Fire' },
  l1    = { 49,               'Power' },
  r1    = { 51,               'Start' },
  l2    = { 50,               'Select' }
}

local menu = {
  { unit1.form1.btn_power_on,    'Power',  49 },
  { unit1.form1.btn_select_down, 'Select', 50 },
  { unit1.form1.btn_start_down,  'Start',  51 }
}

local timers = {
  unit1.form1.timer_demo_1,
  unit1.form1.timer_demo_2,
  unit1.form1.timer_game_start,
  unit1.form1.timer_game_over,
  unit1.form1.timer_keypress,
  unit1.form1.timer_phase123_player_fire,
  unit1.form1.timer_phase123_miss,
  unit1.form1.timer_phase123_enemy_fire,
  unit1.form1.timer_phase1_enemy_row8,
  unit1.form1.timer_phase1_enemy_row7,
  unit1.form1.timer_phase1_enemy_attacking,
  unit1.form1.timer_phase1_enemy_rows78_hit,
  unit1.form1.timer_phase1_enemy_attacking_hit,
  unit1.form1.timer_phase1_reprise,
  unit1.form1.timer_phase1_complete,
  unit1.form1.timer_phase2_enemy_attacking,
  unit1.form1.timer_phase2_reprise,
  unit1.form1.timer_phase2_enemy_attacking_hit,
  unit1.form1.timer_phase2_complete,
  unit1.form1.timer_phase3_mothership,
  unit1.form1.timer_phase3_reprise,
  unit1.form1.timer_phase3_mothership_hit,
  unit1.form1.timer_phase3_complete,
  unit1.form1.timer_phase4_shuttle_thrust,
  unit1.form1.timer_phase4_shuttle_thrust_display,
  unit1.form1.timer_phase4_shuttle_down,
  unit1.form1.timer_phase4_dock,
  unit1.form1.timer_phase4_miss_dock,
  unit1.form1.timer_phase4_bonus
}

unit1.pfs_bonus.data = system.loadbin( 'Bonus.pcm' )
unit1.pfs_complete.data = system.loadbin( 'Complete.pcm' )
unit1.pfs_fire.data = system.loadbin( 'Fire.pcm' )
unit1.pfs_gameon.data = system.loadbin( 'Gameon.pcm' )
unit1.pfs_hit.data = system.loadbin( 'Hit.pcm' )
unit1.pfs_miss.data = system.loadbin( 'Miss.pcm' )
unit1.pfs_missdock.data = system.loadbin( 'Missdock.pcm' )
unit1.pfs_gameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfs_phase.data = system.loadbin( 'Phase.pcm' )
unit1.pfs_selecbtn.data = system.loadbin( 'Selecbtn.pcm' )
unit1.pfs_start.data = system.loadbin( 'Start.pcm' )
unit1.pfs_startbtn.data = system.loadbin( 'Startbtn.pcm' )
unit1.pfs_thrust.data = system.loadbin( 'Thrust.pcm' )
unit1.pfs_tick.data = system.loadbin( 'Tick.pcm' )

if system.GW_VERSION and system.GW_VERSION >= 0x010400 then
  unit1.pfs_thrust.loop = true
end

unit1.form1.oncreate()

unit1.form1.im_background_screen.layer = 0;

unit1.bsound = true

return system.init(
  unit1.form1.im_background,
  keymap,
  function( key ) unit1.form1.onkeydown( nil, key, 0 ) end,
  function( key ) unit1.form1.onkeyup( nil, key, 0 ) end,
  timers,
  { 216, 104, 93, 302 },
  menu
)
