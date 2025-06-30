#include <ft2build.h>
#include FT_FREETYPE_H

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define CLAY_IMPLEMENTATION
#include <clay.h>

#include "cApplication.h"

#ifdef DF_Profiling
	#include <tracy/Tracy.hpp>

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

#ifdef DF_Windows
int WinMain()
{
	df::cApplication::initialize();
	df::cApplication::run();
	df::cApplication::deinitialize();

	return 0;
}
#elif defined( DF_Linux )
int main()
{
	df::cApplication::initialize();
	df::cApplication::run();
	df::cApplication::deinitialize();

	return 0;
}
#endif