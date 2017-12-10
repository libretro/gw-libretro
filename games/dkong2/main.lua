local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  up    = { forms.vk_up,      'Up' },
  down  = { forms.vk_down,    'Down' },
  left  = { forms.vk_left,    'Left' },
  right = { forms.vk_right,   'Right' },
  b     = { forms.vk_control, 'Jump' },
  l1    = { 49,               'Game A' },
  r1    = { 50,               'Game B' },
  l2    = { 51,               'Time' },
  r2    = { 53,               'ACL' }
}

local menu = {
  { unit1.form1.btn_game_a_down, 'Game A', 49 },
  { unit1.form1.btn_game_b_down, 'Game B', 50 },
  { unit1.form1.btn_time_down,   'Time',   51 },
  { unit1.form1.btn_acl_down,    'ACL',    53 }
}

local timers = {
  unit1.form1.timer_bonus_chance,
  unit1.form1.timer_bonus_complete,
  unit1.form1.timer_bonus_lock,
  unit1.form1.timer_clock,
  unit1.form1.timer_game,
  unit1.form1.timer_game_over,
  unit1.form1.timer_game_start,
  unit1.form1.timer_junior_jump,
  unit1.form1.timer_junior_move,
  unit1.form1.timer_key_blink,
  unit1.form1.timer_key_move,
  unit1.form1.timer_miss,
  unit1.form1.timer_score_blink,
  unit1.form1.timer_spark_blink
}

unit1.pfs_chain.data = system.loadbin( 'Chain.pcm' )
unit1.pfs_chance.data = system.loadbin( 'Chance.pcm' )
unit1.pfs_enemy_1.data = system.loadbin( 'Enemy_1.pcm' )
unit1.pfs_enemy_2.data = system.loadbin( 'Enemy_2.pcm' )
unit1.pfs_gameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfs_jump.data = system.loadbin( 'Jump.pcm' )
unit1.pfs_key.data = system.loadbin( 'Key.pcm' )
unit1.pfs_miss.data = system.loadbin( 'Miss.pcm' )
unit1.pfs_safe.data = system.loadbin( 'Safe.pcm' )
unit1.pfs_step.data = system.loadbin( 'Step.pcm' )

unit1.form1.oncreate()

unit1.form1.im_background_closed.picture = nil

unit1.bsound = true
unit1.imode = 2
unit1.form1.gam_set_mode()

local tick = system.init(
  unit1.form1.im_background_open,
  keymap,
  function( key ) unit1.form1.onkeydown( nil, key, 0 ) end,
  function( key ) unit1.form1.onkeyup( nil, key, 0 ) end,
  timers,
  nil,
  menu
)

local state = {}
local last_barrowkeypressed

local newtick = function()
  local active = tick()
  
  if active then
    if unit1.bcontroller ~= last_bcontroller then
      last_barrowkeypressed = unit1.barrowkeypressed

      -- repeat keypresses when barrowkeypressed is true again
      if last_barrowkeypressed then
        system.inputstate( state )

        for button, pressed in pairs( state ) do
          local keys = keymap[ button ]
          
          if keys then
            if pressed then
              for _, key in ipairs( keys ) do
                unit1.form1.onkeydown( nil, key, 0 )
              end
            end
          end
        end
      end
    end
  end

  return active
end

return newtick
