#pragma once

#include <functional>

#include "core/misc/Misc.h"
#include "core/rendering/cShader.h"

#if PROFILING
#include "core/profiling/Profiling.h"
#endif

namespace df
{
    class cShader;

    struct iRenderCallback
    {
        DISABLE_COPY_AND_MOVE( iRenderCallback );

        explicit iRenderCallback( std::string _name )
        : name( std::move( _name ) )
        {}

        virtual ~iRenderCallback() = default;

        const std::string name;
    };

    template< typename... Targs >
    class cRenderCallback final : public iRenderCallback
    {
    public:
        DISABLE_COPY_AND_MOVE( cRenderCallback );

        explicit cRenderCallback( std::string _name, const std::string& _shader_name, void ( _callback )( const cShader*, Targs... ) );
        explicit cRenderCallback( std::string _name, const std::vector< std::string >& _shader_names, void ( _callback )( const cShader*, Targs... ) );
        ~cRenderCallback() override;

        void render( Targs... _args );

    protected:
        std::vector< cShader* >                           m_shaders;
        std::function< void( const cShader*, Targs... ) > m_callback;
    };

    template< typename... Targs >
    cRenderCallback< Targs... >::cRenderCallback( std::string _name, const std::string& _shader_name, void ( _callback )( const cShader*, Targs... ) )
    : iRenderCallback( std::move( _name ) ),
      m_callback( _callback )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        m_shaders.push_back( new cShader( _shader_name ) );
    }

    template< typename... Targs >
    cRenderCallback< Targs... >::cRenderCallback( std::string _name, const std::vector< std::string >& _shader_names, void _callback( const cShader*, Targs... ) )
    : iRenderCallback( std::move( _name ) ),
      m_callback( _callback )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        for( const std::string& shader_name : _shader_names )
            m_shaders.push_back( new cShader( shader_name ) );
    }

    template< typename... Targs >
    cRenderCallback< Targs... >::~cRenderCallback()
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        for( const cShader* shader : m_shaders )
            delete shader;
    }

    template< typename... Targs >
    void cRenderCallback< Targs... >::render( Targs... _args )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        for( cShader* shader : m_shaders )
            m_callback( shader, _args... );
    }
}
