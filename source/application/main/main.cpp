#include <ft2build.h>
#include FT_FREETYPE_H

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "application/application/cApplication.h"

#ifdef DF_Profiling
	#include <tracy/Tracy.hpp>

	#include "engine/profiling/cProfiling.h"

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
	#ifdef DF_Profiling
	df::cProfiling::start();
	#endif

	cApplication::initialize();
	cApplication::run();
	cApplication::deinitialize();

	#ifdef DF_Profiling
	df::cProfiling::stop();
	#endif

	return 0;
}
#elif defined( DF_Linux )
int main()
{
	cApplication::initialize();
	cApplication::run();
	cApplication::deinitialize();

	return 0;
}
#endif