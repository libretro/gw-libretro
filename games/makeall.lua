local dirs = [[
armorbtl
banana
baseball
bombfgt
caccial
cessatef
chickyw
chickywt
condor
crazycwt
defendo
dkong
dkong2
dkongc
dkongcrp
dkongjrc
dkongjrp
dkongjrt
dnd
donkeyan
egg
egg-mg13
egg-mg18
egg-mg33
enginer
escape
galaxyii
grabman
hippot
hippots
hotline
hotlines
lasvegas
lifeboat
mariobro
mariosba
mickeymp
mickeymw
monkey
monkeyj
motorcr
pacmant
pancake
parachut
penguinl
pirate
rollerc
safari
sleepwlk
snoopyp
snoopyt
snoopytn
subchase
tennisms
tjpopper
towering
tron
turtlebr
wildmanj
]]

local function exec( ... )
  local args = { ... }
  local cmd = table.concat( args, '' )
  print(cmd)
  os.execute( cmd )
end

for dir in dirs:gmatch( '(.-)\n' ) do
  exec( 'mkdir ', dir )
  exec( 'cp ', '../../gw-games/' .. dir .. '/main.lua ', dir )
end
