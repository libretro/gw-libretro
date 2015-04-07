local class = system.loadunit 'class'
local graphics = system.loadunit 'graphics'

local M = {}

M.tlabel = class.new()

function M.tlabel:new()
  self.font = graphics.tfont()
end

M.tscrollbar = class.new()

function M.tscrollbar:new()
end

return M
