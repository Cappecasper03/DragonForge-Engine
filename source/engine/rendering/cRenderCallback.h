#pragma once

#include <functional>

#include "iShader.h"
#include "engine/misc/Misc.h"

namespace df
{
    struct iRenderCallback
    {
        DF_DISABLE_COPY_AND_MOVE( iRenderCallback );

        explicit iRenderCallback( std::string _name )
        : name( std::move( _name ) )
        {}

        virtual ~iRenderCallback() = default;

        const std::string name;
    };

    template< typename T, typename... Targs >
    class cRenderCallback final : public iRenderCallback
    {
    public:
        DF_DISABLE_COPY_AND_MOVE( cRenderCallback );

        explicit cRenderCallback( std::string _name, const std::string& _shader_name, void ( _callback )( const T*, Targs... ) );
        explicit cRenderCallback( std::string _name, const std::vector< std::string >& _shader_names, void ( _callback )( const T*, Targs... ) );
        ~cRenderCallback() override;

        void render( Targs... _args );

    protected:
        std::vector< T* >                           m_shaders;
        std::function< void( const T*, Targs... ) > m_callback;
    };

    template< typename T, typename... Targs >
    cRenderCallback< T, Targs... >::cRenderCallback( std::string _name, const std::string& _shader_name, void ( _callback )( const T*, Targs... ) )
    : iRenderCallback( std::move( _name ) ),
      m_callback( _callback )
    {
        ZoneScoped;

        m_shaders.push_back( new T( _shader_name ) );
    }

    template< typename T, typename... Targs >
    cRenderCallback< T, Targs... >::cRenderCallback( std::string _name, const std::vector< std::string >& _shader_names, void _callback( const T*, Targs... ) )
    : iRenderCallback( std::move( _name ) ),
      m_callback( _callback )
    {
        ZoneScoped;

        for( const std::string& shader_name : _shader_names )
            m_shaders.push_back( new T( shader_name ) );
    }

    template< typename T, typename... Targs >
    cRenderCallback< T, Targs... >::~cRenderCallback()
    {
        ZoneScoped;

        for( const iShader* shader : m_shaders )
            delete shader;
    }

    template< typename T, typename... Targs >
    void cRenderCallback< T, Targs... >::render( Targs... _args )
    {
        ZoneScoped;

        for( T* shader : m_shaders )
            m_callback( shader, _args... );
    }
}
