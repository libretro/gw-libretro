static inline uint16_t ne16( uint16_t x )
{
  static const union
  {
    uint16_t u16;
    uint8_t u8[ 2 ];
  }
  u = { 1 };
  
  return u.u8[ 0 ] ? x >> 8 | x << 8 : x;
}

static inline uint32_t ne32( uint32_t x )
{
  static const union
  {
    uint16_t u16;
    uint8_t u8[ 2 ];
  }
  u = { 1 };
  
  if ( u.u8[ 0 ] )
  {
    return ne16( x ) << 16 | ne16( x >> 16 );
  }
  else
  {
    return x;
  }
}
