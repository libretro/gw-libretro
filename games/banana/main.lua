local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'
local controls = system.loadunit 'controls'

unit1.form1.oncreate()

unit1.pfgameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfgot.data = system.loadbin( 'Got.pcm' )
unit1.pfmiss.data = system.loadbin( 'Miss.pcm' )
unit1.pfstart.data = system.loadbin( 'Start.pcm' )
unit1.apfmove[ 1 ].data = system.loadbin( 'Moveup.pcm' )
unit1.apfmove[ 2 ].data = system.loadbin( 'Movedown.pcm' )

unit1.bsound = true

-- this game doesn't have a acl mode so we have to reblit all images
do
  unit1.form1.vid_showall( true )
  unit1.form1.vid_showscore( 8888, true )

  for i = 1, 4 do
    unit1.aim_panel_digits[ i ].visible = true
  end
end

local zoom = { left = 153, top = 54, width = 257, height = 190 }
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
      button = unit1.form1.btn_mode_top,
      label  = 'Time'
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
    unit1.form1.timeralarm,
    unit1.form1.timerclock,
    unit1.form1.timerdate,
    unit1.form1.timerfork,
    unit1.form1.timergame,
    unit1.form1.timergameover,
    unit1.form1.timergamestart,
    unit1.form1.timermiss
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
