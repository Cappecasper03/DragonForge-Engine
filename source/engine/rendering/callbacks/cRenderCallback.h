#pragma once

#include <functional>

#include "engine/misc/Misc.h"
#include "engine/rendering/cShader.h"

namespace df
{
    class cShader;

    struct iRenderCallback
    {
        DF_DISABLE_COPY_AND_MOVE( iRenderCallback );

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
        DF_DISABLE_COPY_AND_MOVE( cRenderCallback );

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
        ZoneScoped;
        
        m_shaders.push_back( new cShader( _shader_name ) );
    }

    template< typename... Targs >
    cRenderCallback< Targs... >::cRenderCallback( std::string _name, const std::vector< std::string >& _shader_names, void _callback( const cShader*, Targs... ) )
    : iRenderCallback( std::move( _name ) ),
      m_callback( _callback )
    {
        ZoneScoped;
        
        for( const std::string& shader_name : _shader_names )
            m_shaders.push_back( new cShader( shader_name ) );
    }

    template< typename... Targs >
    cRenderCallback< Targs... >::~cRenderCallback()
    {
        ZoneScoped;
        
        for( const cShader* shader : m_shaders )
            delete shader;
    }

    template< typename... Targs >
    void cRenderCallback< Targs... >::render( Targs... _args )
    {
        ZoneScoped;
        
        for( cShader* shader : m_shaders )
            m_callback( shader, _args... );
    }
}
