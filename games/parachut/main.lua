local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'
local controls = system.loadunit 'controls'

unit1.form1.oncreate()

unit1.pfs_chance.data = system.loadbin( 'Chance.pcm' )
unit1.pfs_got.data = system.loadbin( 'Got.pcm' )
unit1.pfs_miss1.data = system.loadbin( 'Miss1.pcm' )
unit1.pfs_miss2.data = system.loadbin( 'Miss2.pcm' )
unit1.pfs_miss3.data = system.loadbin( 'Miss3.pcm' )
unit1.pfs_tick.data = system.loadbin( 'Tick.pcm' )

unit1.bsound = true
unit1.imode = 1
unit1.form1.gam_set_mode()

local width = unit1.form1.im_background.width
local height = unit1.form1.im_background.height

return system.init{
  background = unit1.form1.im_background,
  zoom = { 175, 103, 309, 193 },

  controls = {
    { unit1.form1.btn_game_a_top, 'Game A' },
    { unit1.form1.btn_game_b_top, 'Game B' },
    { unit1.form1.btn_time_top,   'Time' },
    { unit1.form1.btn_acl_top,    'ACL' },
    { unit1.form1.btn_left_down,  'Left',  { 'left' },       { forms.vk_left } },
    { unit1.form1.btn_right_down, 'Right', { 'right', 'a' }, { forms.vk_right } },
  },

  timers = {
    unit1.form1.timer_chance_time,
    unit1.form1.timer_clock,
    unit1.form1.timer_game,
    unit1.form1.timer_game_over,
    unit1.form1.timer_miss
  },

  onkey = function( key, pressed )
    local handler = pressed and unit1.form1.onkeydown or unit1.form1.onkeyup
    handler( nil, key, 0 )
  end,

  onbutton = function( button, pressed )
    local handler = pressed and button.onmousedown or button.onmouseup
    handler( nil, controls.mbleft, false, 0, 0 )
  end,

  onmouse = function( x, y, pressed )
    local handler = pressed and unit1.form1.onkeydown or unit1.form1.onkeyup
    local key = ( x < width // 2 ) and forms.vk_left or forms.vk_right
    handler( nil, key, 0 )
  end
}
