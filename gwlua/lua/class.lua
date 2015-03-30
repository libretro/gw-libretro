--[[
Copyright (c) 2010–2015 André Leiradella

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

* The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. 
]]

local M = {}

local pairs = pairs
local type = type
local sub = string.sub
local unpack = table.unpack
local setmetatable = setmetatable
local _G = _G
local methods = {}
setmetatable( methods, { __mode = 'k' } )

function M.new( ... )
  local arg = { ... }
  -- create an empty class
  local new_class = {}
  
  -- copy all methods from the super classes
  for index = 1, #arg do
    for name, method in pairs( arg[ index ] ) do
      if not new_class[ name ] then
        new_class[ name ] = method
      end
    end
  end
  
  -- insert an additional method to check the class of the instance
  new_class.instanceOf = function( self, klass )
    if klass == new_class then
      return true
    end
    
    for index = 1, #arg do
      if arg[ index ].instanceOf( self, klass ) then
        return true
      end
    end
    
    return false
  end
  
  -- insert an additional method to return the class of an instance
  new_class.getClass = function()
    return new_class
  end
  
  -- insert an additional method to return the class name of an instance (only works if the class is accessible from the global space)
  new_class.getClassName = function()
    local function find( space, name, visited )
      if visited[ space ] then
        return nil
      end
      
      visited[ space ] = true
      
      for key, value in pairs( space ) do
        if value == new_class then
          return name .. '.' .. key
        end
        
        if type( value ) == 'table' then
          local found = find( value, name .. '.' .. key, visited )
          
          if found ~= nil then
            return found
          end
        end
      end
      
      return nil
    end
    
    local found = find( _G, '_G', {} )
    
    if found then
      return sub( found, 4, -1 )
    end
    
    return nil
  end
  
  -- the function to index mathods of the class
  local index = function( instance, key )
    -- check the methods cache
    local method = methods[ instance ][ key ]
    
    if not method then
      -- not found, create the method and put it into the cache
      method = function( ... )
        return new_class[ key ]( instance, ... )
      end
      
      methods[ instance ][ key ] = method
    end
    
    return method
  end
  
  -- the metatable of the instances
  local self_meta = { __index = index }
  
  -- turn the self table into an instance of the class
  new_class.makeInstance = function( self )
    -- create the methods cache
    methods[ self ] = {}
    -- set the metatable and return the instance
    return setmetatable( self, self_meta )
  end

  -- create a __call metamethod that creates a new instance of the class
  local class_meta = {}
  
  class_meta.__call = function( ... )
    local arg = { ... }
    
    -- create an empty instance
    local self = {}
    new_class.makeInstance( self )
    
    -- the first argument is the class, shift left all other arguments
    for i = 2, #arg do
      arg[ i - 1 ] = arg[ i ]
    end
    
    arg[ #arg ] = nil
    
    -- call the new method to initialize the instance
    if new_class.new then
      new_class.new( self, unpack( arg ) )
    end
    
    -- return the newly created instance
    return self
  end

  -- set the metatable of the class and return it
  return setmetatable( new_class, class_meta )
end

function M.instanceOf( instance, class )
  return type( instance ) == 'table' and instance.instanceOf and instance.instanceOf( class )
end

function M.abstract()
  local proxy = {}
  
  proxy.__call =  function( proxy, self )
    local klass = self:getClass()
    local methodName = '?'
    
    for name, method in pairs( klass ) do
      if method == proxy then
        methodName = name
        break
      end
    end
    
    error( 'The method ' .. ( self:getClassName() or '?' ) .. '.' .. methodName .. ' is abstract, please implement it.' )
  end
  
  setmetatable( proxy, proxy )
  return proxy
end

return M
