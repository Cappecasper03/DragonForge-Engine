#pragma once
#include <functional>

#include "core/memory/Memory.h"
#include "core/misc/Misc.h"

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

        explicit cRenderCallback( const std::string& _shader_name, const std::function< void( const cShader*, Targs... ) >& _callback );
        ~cRenderCallback() override;

        void render( Targs... _args ) { m_callback( m_shader, _args ); }

    protected:
        cShader*                  m_shader;
        std::function< Targs... > m_callback;
    };

    template< typename... Targs >
    cRenderCallback< Targs... >::cRenderCallback( const std::string& _shader_name, const std::function< void( const cShader*, Targs... ) >& _callback )
    {
        m_shader   = MEMORY_ALLOC( cShader, 1, _shader_name );
        m_callback = _callback;
    }

    template< typename... Targs >
    cRenderCallback< Targs... >::~cRenderCallback()
    {
        MEMORY_FREE( m_shader );
    }
}
