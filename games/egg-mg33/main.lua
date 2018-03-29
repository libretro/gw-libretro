local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'
local controls = system.loadunit 'controls'

unit1.form1.oncreate()

unit1.apfbeeps[ 1 ].data = system.loadbin( 'Move_Nw.pcm' )
unit1.apfbeeps[ 2 ].data = system.loadbin( 'Move_Ne.pcm' )
unit1.apfbeeps[ 3 ].data = system.loadbin( 'Move_Sw.pcm' )
unit1.apfbeeps[ 4 ].data = system.loadbin( 'Move_Se.pcm' )
unit1.pfgameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfmiss.data = system.loadbin( 'Miss.pcm' )
unit1.pfpoint.data = system.loadbin( 'Point.pcm' )

unit1.bsound = true
unit1.imode = 1
unit1.form1.gam_set_mode()

local zoom = { left = 175, top = 96, width = 305, height = 207 }
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
      button = unit1.form1.btn_1_nw_top,
      zone   = nw,
      label  = 'Northwest',
      keys   = { up = true },
      xkeys  = { forms.vk_up, forms.vk_left }
    },
    {
      button = unit1.form1.btn_3_sw_top,
      zone   = sw,
      label  = 'Southwest',
      keys   = { down = true },
      xkeys  = { forms.vk_down, forms.vk_left }
    },
    {
      button = unit1.form1.btn_2_ne_top,
      zone   = ne,
      label  = 'Northeast',
      keys   = { x = true },
      xkeys  = { forms.vk_up, forms.vk_right }
    },
    {
      button = unit1.form1.btn_4_se_top,
      zone   = se,
      label  = 'Southeast',
      keys   = { b = true },
      xkeys  = { forms.vk_down, forms.vk_right }
    }
  },

  timers = {
    unit1.form1.timerchance,
    unit1.form1.timerclock,
    unit1.form1.timerenemy,
    unit1.form1.timergameover,
    unit1.form1.timermisslong,
    unit1.form1.timermissshort,
    unit1.form1.timerpoint,
    unit1.form1.timership,
    unit1.form1.timershowmiss
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
