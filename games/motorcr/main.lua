local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  b     = { forms.vk_up,      'Accelerate' },
  y     = { forms.vk_down,    'Break' },
  left  = { forms.vk_left,    'Left' },
  right = { forms.vk_right,   'Right' },
  a     = { forms.vk_control, 'Start' },
  l1    = { 49,               'On/Off' },
  r1    = { 50,               'Sound' },
  r2    = { 53,               'ACL' }
}

local menu = {
  { unit1.form1.btn_power_down, 'On/Off', 49 },
  { unit1.form1.btn_sound_down, 'Sound', 50 },
  { unit1.form1.btn_acl_down,   'ACL',   53 },
  { unit1.form1.btn_start_down, 'Start', forms.vk_control }
}

local timers = {
  unit1.form1.timer_fuel,
  unit1.form1.timer_fuel_pump,
  unit1.form1.timer_game,
  unit1.form1.timer_game_over,
  unit1.form1.timer_game_start,
  unit1.form1.timer_miss,
  unit1.form1.timer_spanner,
  unit1.form1.timer_stage_complete,
  unit1.form1.timer_track
}

unit1.pfs_gameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfs_miss.data = system.loadbin( 'Miss.pcm' )
unit1.pfs_semaph.data = system.loadbin( 'Semaph.pcm' )
unit1.pfs_service1.data = system.loadbin( 'Service1.pcm' )
unit1.pfs_service2.data = system.loadbin( 'Service2.pcm' )
unit1.pfs_stage.data = system.loadbin( 'Stage.pcm' )
unit1.pfs_start.data = system.loadbin( 'Start.pcm' )
unit1.apfengine[ 1 ].data = system.loadbin( 'Engine1.pcm' )
unit1.apfengine[ 2 ].data = system.loadbin( 'Engine2.pcm' )
unit1.apfengine[ 3 ].data = system.loadbin( 'Engine3.pcm' )

if system.GW_VERSION and system.GW_VERSION >= 0x010400 then
  unit1.apfengine[ 1 ].loop = true
  unit1.apfengine[ 2 ].loop = true
  unit1.apfengine[ 3 ].loop = true
end

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
  { 138, 104, 205, 154 },
  menu
)
