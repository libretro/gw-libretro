local class = system.loadunit 'class'

local M = {}

M.tfont = class.new()

function M.tfont:new()
end

M.tpicture = system.newpicture

M.default_charset = 0
M.clwhite = 0xffffffff
M.clfuchsia = 0xffff00ff
M.clwindowtext = 0xff000000

return M
