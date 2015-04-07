local class = system.loadunit 'class'
local graphics = system.loadunit 'graphics'

local M = {}

M.tform = class.new()

function M.tform:new()
  --self.popupmenu = M.tform()
  self.font = graphics.tfont()
end

function M.tform:show()
end

function M.tform:onactivate()
end

function M.tform:onclose()
end

function M.tform:oncreate()
end

function M.tform:onkeydown()
end

function M.tform:onkeyup()
end

M.tcloseaction = class.new()

function M.tcloseaction:new()
end

M.cafree = 0
M.bsnone = 0
M.ponone = 0
M.vk_up = -1
M.vk_down = -2
M.vk_left = -3
M.vk_right = -4
M.vk_control = -5
M.vk_menu = -6
M.vk_shift = -7
M.vk_insert = -8

return M
