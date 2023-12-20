#pragma once

#include <functional>

#include "core/misc/Misc.h"
#include "core/rendering/cShader.h"

namespace df
{
    class cShader;

    struct iRenderCallback
    {
        DISABLE_COPY_AND_MOVE( iRenderCallback );

        iRenderCallback()          = default;
        virtual ~iRenderCallback() = default;
    };

    template< typename... Targs >
    class cRenderCallback final : public iRenderCallback
    {
    public:
        DISABLE_COPY_AND_MOVE( cRenderCallback );

        explicit cRenderCallback( const std::string& _shader_name, void ( _callback )( const cShader*, Targs... ) );
        explicit cRenderCallback( const std::vector< std::string >& _shader_names, void ( _callback )( const cShader*, Targs... ) );
        ~cRenderCallback() override;

        void render( Targs... _args );

    protected:
        std::vector< cShader* >                           m_shaders;
        std::function< void( const cShader*, Targs... ) > m_callback;
    };

    template< typename... Targs >
    cRenderCallback< Targs... >::cRenderCallback( const std::string& _shader_name, void ( _callback )( const cShader*, Targs... ) )
    : m_callback( _callback )
    {
        m_shaders.push_back( new cShader( _shader_name ) );
    }

    template< typename... Targs >
    cRenderCallback< Targs... >::cRenderCallback( const std::vector< std::string >& _shader_names, void _callback( const cShader*, Targs... ) )
    : m_callback( _callback )
    {
        for( const std::string& shader_name : _shader_names )
            m_shaders.push_back( new cShader( shader_name ) );
    }

    template< typename... Targs >
    cRenderCallback< Targs... >::~cRenderCallback()
    {
        for( const cShader* shader : m_shaders )
            delete shader;
    }

    template< typename... Targs >
    void cRenderCallback< Targs... >::render( Targs... _args )
    {
        for( cShader* shader : m_shaders )
            m_callback( shader, _args... );
    }
}
