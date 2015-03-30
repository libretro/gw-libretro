local fmodtypes = system.loadunit 'fmodtypes'

local M = {}

M.fsound_sample_free = function()
end

M.fsound_close = function()
end

M.fsound_playsound = function( what, pfs )
  if what == fmodtypes.fsound_free then
    system.playsound( pfs )
  else
    error( 'fsound_playsound can only be called with fsound_free' )
  end
end

M.fsound_stopsound = function( what )
  if what == fmodtypes.fsound_all then
    system.stopsounds()
  else
    error( 'fsound_stopsound can only be called with fsound_all' )
  end
end

return M
