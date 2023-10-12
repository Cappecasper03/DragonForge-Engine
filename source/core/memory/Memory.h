#pragma once

namespace Memory
{
    extern float getUsageKb();
    extern float getUsagePeakKb();
}

#if defined( DEBUG )
#undef new


// TODO: Custom functions instead of overloads
extern void* operator new( const size_t _size, const char* _file, const int _line, const char* _function );
extern void* operator new[]( const size_t _size, const char* _file, const int _line, const char* _function );

extern void operator delete( void* _address, const char* _file, const int _line, const char* _function );
extern void operator delete[]( void* _address, const char* _file, const int _line, const char* _function );

#define NEW    new   ( __FILE__, __LINE__, __FUNCTION__ )
#define DELETE delete( __FILE__, __LINE__, __FUNCTION__ )

#else

#define NEW    new;
#define DELETE delete;

#endif
