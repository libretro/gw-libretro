local cache = {}

return function( M )
  M.loadunit = function( name )
    name = name .. '.lua'
    local unit = cache[ name ]
    
    if unit then
      return unit
    end
    
    local bin = M.loadbin( name )
    
    if bin then
      local err
      unit, err = load( bin, name, 't' )
      
      if not unit then
        error( err )
      end
      
      unit = unit()
      cache[ name ] = unit
      return unit
    end
    
    error( 'unit ' .. name .. ' not found' )
  end
  
  local class = M.loadunit 'class'
  
  M.tobject = class.new()

  function M.tobject:new()
  end
end
