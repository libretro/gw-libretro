local cache = {}

return function( M )
  M.loadunit = function( name )
    local entry = name .. '.lua'
    local unit = cache[ entry ]
    
    if unit then
      return unit
    end
    print( '@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@' )
    print( 1, entry )
    local bin = M.loadbin( entry )
    print( 2, bin )
    if not bin then
      bin = M.loadbs( name .. '.bs' )
      print( 3, bin )
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
