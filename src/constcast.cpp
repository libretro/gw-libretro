extern "C" void* constcast( const void* ptr )
{
  return const_cast< void* >( ptr );
}
