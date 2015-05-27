local class = system.loadunit 'class'

local M = {}

M.treginifile = class.new()

function M.treginifile:new()
end

function M.treginifile:create()
  return M.treginifile()
end

function M.treginifile:readinteger( arg1, key, value )
  return system.loadvalue( key:lower() ) or value
end

function M.treginifile:writeinteger( arg1, key, value)
  system.savevalue( key:lower(), value )
end

return M
