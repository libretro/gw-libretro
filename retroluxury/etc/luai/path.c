#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <lua.h>
#include <lauxlib.h>

_CRTIMP char __cdecl *realpath( const char *__restrict__ name, char *__restrict__ resolved );

#define SEPARATOR '/'
#define OTHER_SEP '\\'

static int l_realpath( lua_State* L )
{
  const char* path = luaL_checkstring( L, 1 );
  char        buffer[ _MAX_PATH ];
  char*       resolved = realpath( path, buffer );
  
  if ( resolved != NULL )
  {
    while ( *resolved != 0 )
    {
      if ( *resolved == OTHER_SEP )
      {
        *resolved = SEPARATOR;
      }
      
      resolved++;
    }
    
    lua_pushstring( L, buffer );
    return 1;
  }
  
  lua_pushstring( L, strerror( errno ) );
  return lua_error( L );
}

static int is_separator( char k )
{
  return k == SEPARATOR || k == OTHER_SEP;
}

/*static int l_sanitize( lua_State* L )
{
  size_t length;
  const char* path = luaL_checklstring( L, 1, &length );
  
  if ( length < _MAX_PATH - 1 )
  {
    char source[ _MAX_PATH + 1 ];
    source[ 0 ] = SEPARATOR;
    memcpy( source + 1, path, length );
    
    char* dest = source;
    
    for ( size_t i = 1; i <= length; )
    {
      if ( source[ i ] == '\\' && source[ i + 1 ] == ' ' )
      {
        *dest++ = ' ';
      }
      else if ( IsSeparator( source[ i ] ) )
      {
        if ( source[ i + 1 ] == '.' )
        {
          if ( IsSeparator( source[ i + 2 ] ) )
          {
            *dest++ = SEPARATOR;
            i += 3;
          }
          else if ( source[ i + 2 ] == '.' && IsSeparator( source[ i + 3 ] ) )
          {
            while ( *dest != SEPARATOR )
            {
              dest--;
            }
            
            if ( dest == source + 1 )
            {
              return luaL_error( L, "Invalid path" );
            }
            
            dest++;
            i += 4;
          }
        }
        else if ( !IsSeparator( dest[ -1 ] ) )
        {
          *dest++ = SEPARATOR;
          i++;
        }
      }
      else
      {
        *dest++ = source[ i++ ];
      }
    }
    
    lua_pushlstring( L, source + 1, dest - source - 1 );
    return 1;
  }
  
  return luaL_error( L, "Path too long" );
}
*/

static int l_split( lua_State* L )
{
  size_t      length;
  const char* path = luaL_checklstring( L, 1, &length );
  const char* ext = path + length;
  
  while ( ext >= path && *ext != '.' && !is_separator( *ext ) )
  {
    ext--;
  }
  
  const char* name = ext;
  
  if ( *ext != '.' )
  {
    ext = NULL;
  }
  
  while ( name >= path && !is_separator( *name ) )
  {
    name--;
  }

  if ( is_separator( *name ) )
  {
    name++;
  }

  if ( name - path - 1 > 0 )
  {
    lua_pushlstring( L, path, name - path - 1 );
  }
  else
  {
    lua_pushliteral( L, "" );
  }
  
  if ( ext != NULL )
  {
    lua_pushlstring( L, name, ext - name );
    lua_pushstring( L, ext );
  }
  else
  {
    lua_pushstring( L, name );
    lua_pushliteral( L, "" );
  }
  
  return 3;
}

LUAMOD_API int luaopen_path( lua_State* L )
{
  static const luaL_Reg statics[] =
  {
    { "realpath", l_realpath },
    //{ "sanitize", l_sanitize },
    { "split", l_split },
    { NULL, NULL }
  };

  luaL_newlib( L, statics );
  
  lua_pushfstring( L, "%c", SEPARATOR );
  lua_setfield( L, -2, "separator" );
  
  return 1;
}
