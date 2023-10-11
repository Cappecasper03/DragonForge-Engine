#pragma once

#include <cassert>

template< typename T >
class iSingleton
{
public:
    iSingleton( const iSingleton& )     = delete;
    iSingleton( iSingleton&& )          = delete;
    void operator=( const iSingleton& ) = delete;
    void operator=( iSingleton&& )      = delete;

    iSingleton() { s_instance = reinterpret_cast< T* >( this ); }
    virtual ~iSingleton() { s_instance = nullptr; }

    template< typename... Params >
    static void initialize( Params... _params )
    {
        assert( !s_instance, "Singleton is already initialized" );
        s_instance = new T( _params... );
    }

    static void deinitialize()
    {
        assert( s_instance, "No singleton initialized" );
        s_instance = nullptr;
    }

    static T* getInstance() { return s_instance; }

private:
    static T* s_instance;
};
