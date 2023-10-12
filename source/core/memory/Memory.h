#pragma once

namespace Memory
{
    extern float getUsageKb();
    extern float getUsagePeakKb();

    extern void track( void* _address, const size_t _size, const char* _file, const int _line, const char* _function );

    template< typename T, typename... Params >
    extern T* alloc( const unsigned _amount, const char* _file, const int _line, const char* _function, Params... _params )
    {
        T* address = nullptr;

        if( _amount > 1 )
            address = new T[ _amount ]( _params... );
        else
            address = new T( _params... );

        track( address, sizeof( T ) * _amount, _file, _line, _function );

        return address;
    }

    extern void free( void* _address, const char* _file, const int _line, const char* _function );
}

#define ALLOC( _type, _amount, ... ) Memory::alloc<_type>( _amount, __FILE__, __LINE__, __FUNCTION__,  __VA_ARGS__ )
#define FREE( _address ) Memory::free( _address, __FILE__, __LINE__, __FUNCTION__ )
