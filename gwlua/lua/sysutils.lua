local M = {}

M.now = system.now
M.decodetime = system.splittime
M.inttostr = system.inttostr

function M.decodedate( time )
  local hour, min, sec, msec, day, month, year = system.splittime( time )
  return day, month, year
end

return M
