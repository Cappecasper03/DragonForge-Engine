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
        template< typename... Targs >
        static cRenderCallback< Targs... >* create( const std::string& _callback_name, const std::vector< std::string >& _shader_names, void ( _callback )( const cShader*, Targs... ) );

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
#if defined( PROFILING )
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

        if( render_callbacks.contains( _shader_name ) )
        {
            LOG_WARNING( std::format( "Callback already exist: {}", _shader_name ) );
            return nullptr;
        }

        cRenderCallback< Targs... >* callback = new cRenderCallback< Targs... >( _shader_name, _shader_name, _callback );
        render_callbacks[ _shader_name ]      = callback;

        LOG_MESSAGE( std::format( "Created callback: {}", _shader_name ) );
        return callback;
    }

    template< typename... Targs >
    cRenderCallback< Targs... >* cRenderCallbackManager::create( const std::string& _callback_name, const std::vector< std::string >& _shader_names, void _callback( const cShader*, Targs... ) )
    {
#if defined( PROFILING )
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

        if( render_callbacks.contains( _callback_name ) )
        {
            LOG_WARNING( std::format( "Callback already exist: {}", _callback_name ) );
            return nullptr;
        }

        cRenderCallback< Targs... >* callback = new cRenderCallback< Targs... >( _callback_name, _shader_names, _callback );
        render_callbacks[ _callback_name ]    = callback;

        LOG_MESSAGE( std::format( "Created callback: {}", _callback_name ) );
        return callback;
    }

    inline bool cRenderCallbackManager::destroy( const std::string& _name )
    {
#if defined( PROFILING )
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

        const auto it = render_callbacks.find( _name );
        if( it == render_callbacks.end() )
        {
            LOG_WARNING( std::format( "Callback doesn't exist: {}", _name ) );
            return false;
        }

        delete it->second;
        render_callbacks.erase( it );
        LOG_MESSAGE( std::format( "Destroyed callback: {}", _name ) );

        return true;
    }

    inline bool cRenderCallbackManager::destroy( const iRenderCallback* _callback )
    {
#if defined( PROFILING )
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        if( !_callback )
            return false;

        std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

        for( const std::pair< const std::string, iRenderCallback* >& callback : render_callbacks )
        {
            if( callback.second == _callback )
            {
                LOG_MESSAGE( std::format( "Destroyed callback: {}", callback.first ) );
                delete callback.second;
                render_callbacks.erase( callback.first );
                return true;
            }
        }

        LOG_WARNING( std::format( "Callback isn't managed: {}", _callback->name ) );
        return false;
    }

    inline void cRenderCallbackManager::clear()
    {
#if defined( PROFILING )
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

        for( const std::pair< const std::string, iRenderCallback* >& callback : render_callbacks )
        {
            if( callback.second )
            {
                LOG_MESSAGE( std::format( "Destroyed callback: {}", callback.first ) );
                delete callback.second;
            }
        }

        render_callbacks.clear();
    }

    template< typename... Targs >
    void cRenderCallbackManager::render( const std::string& _name, Targs... _args )
    {
#if defined( PROFILING )
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        cRenderCallback< Targs... >* callback = reinterpret_cast< cRenderCallback< Targs... >* >( getInstance()->m_render_callbacks[ _name ] );
        if( callback )
            callback->render( _args... );
    }

    template< typename... Targs >
    void cRenderCallbackManager::render( iRenderCallback* _callback, Targs... _args )
    {
#if defined( PROFILING )
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        cRenderCallback< Targs... >* callback = reinterpret_cast< cRenderCallback< Targs... >* >( _callback );
        if( callback )
            callback->render( _args... );
    }

    inline iRenderCallback* cRenderCallbackManager::get( const std::string& _name )
    {
#if defined( PROFILING )
        PROFILING_SCOPE( __FUNCTION__ );
#endif

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
