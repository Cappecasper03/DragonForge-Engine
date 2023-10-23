#pragma once

#include <format>

#include "core/misc/iSingleton.h"
#include "core/rendering/callbacks/cRenderCallback.h"
#include "core/rendering/callbacks/DefaultQuadCB.h"

namespace df
{
    struct iRenderCallback;

    class cRenderCallbackManager : public iSingleton< cRenderCallbackManager >
    {
    public:
        DISABLE_COPY_AND_MOVE( cRenderCallbackManager );

        cRenderCallbackManager();
        ~cRenderCallbackManager() override { clear(); }

        template< typename... Targs >
        cRenderCallback< Targs... >* create( const std::string& _callback_name, const std::string& _shader_name, void ( _callback )( const cShader*, Targs... ) );
        bool                         add( const std::string& _name, iRenderCallback* _callback );

        bool destroy( const std::string& _name );
        bool destroy( const iRenderCallback* _callback );
        void clear();

        template< typename... Targs >
        cRenderCallback< Targs... >* get( const std::string& _name );

    private:
        std::unordered_map< std::string, iRenderCallback* > m_render_callbacks;
    };

    inline cRenderCallbackManager::cRenderCallbackManager()
    {
        create( "defaultQuad", "default_quad", render_callback::defaultQuad );
    }

    template< typename... Targs >
    cRenderCallback< Targs... >* cRenderCallbackManager::create( const std::string& _callback_name, const std::string& _shader_name, void _callback( const cShader*, Targs... ) )
    {
        if( m_render_callbacks.contains( _callback_name ) )
        {
            LOG_WARNING( std::format( "Callback already exist: {}", _callback_name ) );
            return nullptr;
        }

        cRenderCallback< Targs... >* callback = MEMORY_ALLOC( cRenderCallback<Targs...>, 1, _shader_name, _callback );
        m_render_callbacks[ _callback_name ]  = callback;

        LOG_MESSAGE( std::format( "Created callback: {}", _callback_name ) );
        return callback;
    }

    inline bool cRenderCallbackManager::add( const std::string& _name, iRenderCallback* _callback )
    {
        if( m_render_callbacks.contains( _name ) )
        {
            LOG_WARNING( std::format( "Callback already exist: {}", _name ) );
            return false;
        }

        LOG_MESSAGE( std::format( "Added callback: {}", _name ) );
        m_render_callbacks[ _name ] = _callback;
        return true;
    }

    inline bool cRenderCallbackManager::destroy( const std::string& _name )
    {
        if( m_render_callbacks.erase( _name ) )
        {
            LOG_MESSAGE( std::format( "Destroyed callback: {}", _name ) );
            return true;
        }

        LOG_WARNING( std::format( "Callback doesn't exist: {}", _name ) );
        return false;
    }

    inline bool cRenderCallbackManager::destroy( const iRenderCallback* _callback )
    {
        for( const std::pair< const std::string, iRenderCallback* >& callback : m_render_callbacks )
        {
            if( callback.second == _callback )
            {
                m_render_callbacks.erase( callback.first );
                LOG_MESSAGE( std::format( "Destroyed callback: {}", callback.first ) );
                return true;
            }
        }

        return false;
    }

    inline void cRenderCallbackManager::clear()
    {
        for( const std::pair< const std::string, iRenderCallback* >& callback : m_render_callbacks )
        {
            if( callback.second )
            {
                LOG_MESSAGE( std::format( "Destroyed callback: {}", callback.first ) );
                MEMORY_FREE( callback.second );
            }
        }

        m_render_callbacks.clear();
    }

    template< typename... Targs >
    cRenderCallback< Targs... >* cRenderCallbackManager::get( const std::string& _name )
    {
        const auto it = m_render_callbacks.find( _name );
        if( it == m_render_callbacks.end() )
        {
            LOG_WARNING( std::format( "Callback doesn't exist: {}", _name ) );
            return nullptr;
        }

        return it->second;
    }
}
