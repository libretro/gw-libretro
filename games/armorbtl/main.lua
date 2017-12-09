local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  up    = { forms.vk_up,      'Up' },
  down  = { forms.vk_down,    'Down' },
  left  = { forms.vk_left,    'Left' },
  right = { forms.vk_right,   'Right' },
  b     = { forms.vk_control, 'Fire' },
  l1    = { 49,               'On/Off' }
}

local menu = {
  { unit1.form1.btn_power_top, 'On/Off', 49 }
}

local timers = {
  unit1.form1.timer_enemy_blink,
  unit1.form1.timer_game_over,
  unit1.form1.timer_game_sound_tick,
  unit1.form1.timer_game_time,
  unit1.form1.timer_miss,
  unit1.form1.timer_missed_shot,
  unit1.form1.timer_reprise,
}

unit1.pfs_adjacent.data = system.loadbin( 'Adjacent.pcm' )
unit1.pfs_enemy.data = system.loadbin( 'Enemy.pcm' )
unit1.pfs_fire.data = system.loadbin( 'Fire.pcm' )
unit1.pfs_hit.data = system.loadbin( 'Hit.pcm' )
unit1.pfs_mine.data = system.loadbin( 'Mine.pcm' )
unit1.pfs_gameover.data = system.loadbin( 'GameOver.pcm' )
unit1.pfs_tick.data = system.loadbin( 'Tick.pcm' )

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
  { 128, 168, 240, 272 },
  menu
)
