local M = {}

-- Every image the game creates, in creation order. The games were
-- desktop applications and their controls still carry their original
-- onmousedown/onmouseup handlers; compatinit hit-tests pointer clicks
-- against this list to deliver them.
local images = {}

M.images = images

M.timage = function( ... )
  local image = system.newimage( ... )
  images[ #images + 1 ] = image
  return image
end

M.ttimer = system.newtimer

return M
