local cache = {}

return function( M )
  M.loadunit = function( name )
    local entry = name .. '.lua'
    local unit = cache[ entry ]
    
    if unit then
      return unit
    end

    local bin = M.loadbin( entry )

    if not bin then
      bin = M.loadbs( name .. '.bs' )
    end
    
    if bin then
      local err
      unit, err = load( bin, entry, 't' )
      
      if not unit then
        error( err )
      end
      
      unit = unit()
      cache[ entry ] = unit
      return unit
    end
    
    error( 'unit ' .. entry .. ' not found' )
  end
  
  local class = M.loadunit 'class'
  
  M.tobject = class.new()

  function M.tobject:new()
  end
end
