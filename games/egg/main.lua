local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'
local controls = system.loadunit 'controls'

unit1.form1.oncreate()

unit1.apfs_move[ 1 ].data = system.loadbin( 'Move_nw.pcm' )
unit1.apfs_move[ 2 ].data = system.loadbin( 'Move_ne.pcm' )
unit1.apfs_move[ 3 ].data = system.loadbin( 'Move_sw.pcm' )
unit1.apfs_move[ 4 ].data = system.loadbin( 'Move_se.pcm' )
unit1.pfs_gameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfs_miss.data = system.loadbin( 'Miss.pcm' )
unit1.pfs_point.data = system.loadbin( 'Point.pcm' )

unit1.bsound = true
unit1.imode = 1
unit1.form1.gam_set_mode()

local zoom = { left = 175, top = 103, width = 309, height = 193 }
local left, right = system.splith( zoom )
local nw, sw = system.splitv( left )
local ne, se = system.splitv( right )

return system.init{
  background = unit1.form1.im_background,
  zoom = zoom,

  controls = {
    {
      button = unit1.form1.btn_game_a_top,
      label  = 'Game A'
    },
    {
      button = unit1.form1.btn_game_b_top,
      label  = 'Game B'
    },
    {
      button = unit1.form1.btn_time_top,
      label  = 'Time'
    },
    {
      button = unit1.form1.btn_acl_top,
      label  = 'ACL'
    },
    {
      button = unit1.form1.btn_1_nw_down,
      zone   = nw,
      label  = 'Northwest',
      keys   = { up = true },
      xkeys  = { forms.vk_up, forms.vk_left }
    },
    {
      button = unit1.form1.btn_3_sw_down,
      zone   = sw,
      label  = 'Southwest',
      keys   = { down = true },
      xkeys  = { forms.vk_down, forms.vk_left }
    },
    {
      button = unit1.form1.btn_2_ne_down,
      zone   = ne,
      label  = 'Northeast',
      keys   = { x = true },
      xkeys  = { forms.vk_up, forms.vk_right }
    },
    {
      button = unit1.form1.btn_4_se_down,
      zone   = se,
      label  = 'Southeast',
      keys   = { b = true },
      xkeys  = { forms.vk_down, forms.vk_right }
    }
  },

  timers = {
    unit1.form1.timer_chance,
    unit1.form1.timer_clock,
    unit1.form1.timer_game,
    unit1.form1.timer_game_over,
    unit1.form1.timer_miss_long,
    unit1.form1.timer_miss_short,
    unit1.form1.timer_point,
    unit1.form1.timer_observer,
    unit1.form1.timer_show_miss
  },

  onkey = function( key, pressed )
    local handler = pressed and unit1.form1.onkeydown or unit1.form1.onkeyup
    handler( nil, key, 0 )
  end,

  onbutton = function( button, pressed )
    local handler = pressed and button.onmousedown or button.onmouseup
    handler( nil, controls.mbleft, false, 0, 0 )
  end
}
