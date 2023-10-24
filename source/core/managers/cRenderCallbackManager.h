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

        cRenderCallbackManager() = default;
        ~cRenderCallbackManager() override { clear(); }

        template< typename... Targs >
        static cRenderCallback< Targs... >* create( const std::string& _shader_name, void ( _callback )( const cShader*, Targs... ) );
        static bool                         add( const std::string& _name, iRenderCallback* _callback );

        static bool destroy( const std::string& _name );
        static bool destroy( const iRenderCallback* _callback );
        static void clear();

        template< typename... Targs >
        static void render( const std::string& _name, Targs... _args );
        template< typename... Targs >
        static void render( iRenderCallback* _callback, Targs... _args );

        template< typename... Targs >
        static cRenderCallback< Targs... >* get( const std::string& _name ) { return get( _name ); }

        static iRenderCallback* get( const std::string& _name );

    private:
        std::unordered_map< std::string, iRenderCallback* > m_render_callbacks;
    };

    template< typename... Targs >
    cRenderCallback< Targs... >* cRenderCallbackManager::create( const std::string& _shader_name, void _callback( const cShader*, Targs... ) )
    {
        std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

        if( render_callbacks.contains( _shader_name ) )
        {
            LOG_WARNING( std::format( "Callback already exist: {}", _shader_name ) );
            return nullptr;
        }

        cRenderCallback< Targs... >* callback = MEMORY_ALLOC( cRenderCallback<Targs...>, 1, _shader_name, _callback );
        render_callbacks[ _shader_name ]      = callback;

        LOG_MESSAGE( std::format( "Created callback: {}", _shader_name ) );
        return callback;
    }

    inline bool cRenderCallbackManager::add( const std::string& _name, iRenderCallback* _callback )
    {
        std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

        if( render_callbacks.contains( _name ) )
        {
            LOG_WARNING( std::format( "Callback already exist: {}", _name ) );
            return false;
        }

        LOG_MESSAGE( std::format( "Added callback: {}", _name ) );
        render_callbacks[ _name ] = _callback;
        return true;
    }

    inline bool cRenderCallbackManager::destroy( const std::string& _name )
    {
        std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

        if( render_callbacks.erase( _name ) )
        {
            LOG_MESSAGE( std::format( "Destroyed callback: {}", _name ) );
            return true;
        }

        LOG_WARNING( std::format( "Callback doesn't exist: {}", _name ) );
        return false;
    }

    inline bool cRenderCallbackManager::destroy( const iRenderCallback* _callback )
    {
        std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

        for( const std::pair< const std::string, iRenderCallback* >& callback : render_callbacks )
        {
            if( callback.second == _callback )
            {
                render_callbacks.erase( callback.first );
                LOG_MESSAGE( std::format( "Destroyed callback: {}", callback.first ) );
                return true;
            }
        }

        return false;
    }

    inline void cRenderCallbackManager::clear()
    {
        std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

        for( const std::pair< const std::string, iRenderCallback* >& callback : render_callbacks )
        {
            if( callback.second )
            {
                LOG_MESSAGE( std::format( "Destroyed callback: {}", callback.first ) );
                MEMORY_FREE( callback.second );
            }
        }

        render_callbacks.clear();
    }

    template< typename... Targs >
    void cRenderCallbackManager::render( const std::string& _name, Targs... _args )
    {
        cRenderCallback< Targs... >* callback = reinterpret_cast< cRenderCallback< Targs... >* >( getInstance()->m_render_callbacks[ _name ] );
        if( callback )
            callback->render( _args... );
    }

    template< typename... Targs >
    void cRenderCallbackManager::render( iRenderCallback* _callback, Targs... _args )
    {
        cRenderCallback< Targs... >* callback = reinterpret_cast< cRenderCallback< Targs... >* >( _callback );
        if( callback )
            callback->render( _args... );
    }

    inline iRenderCallback* cRenderCallbackManager::get( const std::string& _name )
    {
        std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

        const auto it = render_callbacks.find( _name );
        if( it == render_callbacks.end() )
        {
            LOG_WARNING( std::format( "Callback doesn't exist: {}", _name ) );
            return nullptr;
        }

        return it->second;
    }
}
