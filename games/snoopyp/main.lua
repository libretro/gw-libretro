local unit1 = system.loadunit 'unit1'
local forms = system.loadunit 'forms'

local keymap = {
  left  = { forms.vk_left,    'Left' },
  right = { forms.vk_right,   'Right' },
  b     = { forms.vk_control, 'Hit' },
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
  unit1.form1.timer_clock,
  unit1.form1.timer_game,
  unit1.form1.timer_game_over,
  unit1.form1.timer_game_start,
  unit1.form1.timer_intermission,
  unit1.form1.timer_intermission_no_chance,
  unit1.form1.timer_miss_fall,
  unit1.form1.timer_miss_note_snoopy,
  unit1.form1.timer_miss_note_woodstock,
  unit1.form1.timer_score_blink,
  unit1.form1.timer_snoopy_hammer,
  unit1.form1.timer_snoopy_hit_note,
  unit1.form1.timer_snoopy_jump,
  unit1.form1.timer_woodstock,
}

unit1.pfs_chance.data = system.loadbin( 'Chance.pcm' )
unit1.pfs_gameover.data = system.loadbin( 'Gameover.pcm' )
unit1.pfs_hammer1.data = system.loadbin( 'Hammer1.pcm' )
unit1.pfs_hammer2.data = system.loadbin( 'Hammer2.pcm' )
unit1.pfs_interm1.data = system.loadbin( 'Interm1.pcm' )
unit1.pfs_interm2.data = system.loadbin( 'Interm2.pcm' )
unit1.pfs_jump.data = system.loadbin( 'Jump.pcm' )
unit1.pfs_miss1.data = system.loadbin( 'Miss1.pcm' )
unit1.pfs_miss2.data = system.loadbin( 'Miss2.pcm' )
unit1.pfs_start.data = system.loadbin( 'Start.pcm' )

for i = 1, 28 do
  unit1.apfnote[ i ].data = system.loadbin( string.format( 'Note%d.pcm', i ) )
end

for i = 1, 4 do
  unit1.aim_woodstock[ i ].snore = {}
end

unit1.form1.oncreate()
unit1.form1.im_background_closed.visible = false
unit1.form1.im_background_closed = nil
unit1.form1.im_background_screen.layer = 0

unit1.bsound = true
unit1.imode = 2
unit1.form1.gam_set_mode()

return system.init(
  unit1.form1.im_background,
  keymap,
  function( key ) unit1.form1.onkeydown( nil, key, 0 ) end,
  function( key ) unit1.form1.onkeyup( nil, key, 0 ) end,
  timers,
  { 72, 106, 351, 188 },
  menu
)
