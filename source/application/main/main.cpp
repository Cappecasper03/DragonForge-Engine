#include <tracy/Tracy.hpp>

#ifdef PROFILING
void* operator new( size_t _size )
{
	void* ptr = malloc( _size );
	TracyAlloc( ptr, _size );
	return ptr;
}

void operator delete( void* _ptr ) noexcept
{
	TracyFree( _ptr );
	free( _ptr );
}
#endif

int WinMain()
{
	return 0;
}