local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  -- Penguin
  left        = {forms.vk_left,    'Left'},
  right       = {forms.vk_right,   'Right'},
  up          = {forms.vk_control, 'Attack'},
  b           = {forms.vk_control, 'Penguin Attack'},
  -- Seal
  y           = {52,               'Seal Left'},
  a           = {53,               'Seal Right'},
  x           = {54,               'Seal Attack'},
  -- Functions
  l1          = {49,               'Power'},
  r1          = {50,               'Select'},
  l2          = {51,               'Sound'},
  -- 2nd controller
  ['left/2']  = {52,               'Seal Left'},
  ['right/2'] = {53,               'Seal Right'},
  ['b/2'] =     {54,               'Seal Attack'}
}

local menu = {
  { unit1.form1.btn_power_top,  'Power',  49 },
  { unit1.form1.btn_select_top, 'Select', 50 },
  { unit1.form1.btn_sound_top,  'Sound',  51 }
}

local timers = {
  unit1.form1.timer_block_generate,
  unit1.form1.timer_blocks_bonus,
  unit1.form1.timer_cpu_ai_execute,
  unit1.form1.timer_game,
  unit1.form1.timer_game_miss,
  unit1.form1.timer_game_over,
  unit1.form1.timer_game_start,
  unit1.form1.timer_move_arm_penguin,
  unit1.form1.timer_move_arm_seal,
  unit1.form1.timer_vulture_move,
  unit1.form1.timer_wall_move
}

unit1.pfs_beep.data = system.loadbin('Beep.pcm')
unit1.pfs_block.data = system.loadbin('Block.pcm')
unit1.pfs_bomb.data = system.loadbin('Bomb.pcm')
unit1.pfs_bonus.data = system.loadbin('Bonus.pcm')
unit1.pfs_gameover.data = system.loadbin('Gameover.pcm')
unit1.pfs_got.data = system.loadbin('Got.pcm')
unit1.pfs_start.data = system.loadbin('Start.pcm')
unit1.pfs_throw.data = system.loadbin('Throw.pcm')

unit1.akeys[1].word_ = 49
unit1.akeys[2].word_ = 50
unit1.akeys[3].word_ = 51
unit1.akeys[4].word_ = forms.vk_control
unit1.akeys[5].word_ = forms.vk_left
unit1.akeys[6].word_ = forms.vk_right
unit1.akeys[7].word_ = 52
unit1.akeys[8].word_ = 53
unit1.akeys[9].word_ = 54

unit1.ascore[1] = 0
unit1.ascore[2] = 0

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
  nil,
  menu
)
