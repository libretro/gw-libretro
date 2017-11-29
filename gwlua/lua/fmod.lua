local fmodtypes = system.loadunit 'fmodtypes'

local M = {}

M.fsound_sample_free = function()
end

M.fsound_close = function()
end

M.fsound_playsound = function( channel, pfs )
  system.playsound( pfs, channel )
end

M.fsound_stopsound = function( channel )
  system.stopsounds( channel or fmodtypes.fsound_free )
end

M.fsound_setsfxmastervolume = function( vol )
end

return M
