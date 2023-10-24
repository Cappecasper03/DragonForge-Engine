#pragma once

#include <format>
#include <ranges>
#include <string>
#include <unordered_map>

#include "core/log/Log.h"
#include "core/memory/Memory.h"
#include "core/misc/Misc.h"
#include "core/rendering/assets/iRenderAsset.h"

namespace df
{
    template< typename T, typename Tasset >
    class iRenderAssetManager : public iSingleton< T >
    {
    public:
        DISABLE_COPY_AND_MOVE( iRenderAssetManager );

        iRenderAssetManager();
        ~iRenderAssetManager() override = default;

        template< typename... Targs >
        static Tasset* create( const std::string& _name, Targs... _args );

        static void update( const float& _delta_time );
        static void render();

        static bool add( const std::string& _name, Tasset* _asset );

        static bool destroy( const std::string& _name );
        static bool destroy( const Tasset* _asset );
        static void clear();

        static Tasset* get( const std::string& _name );

        static iRenderCallback* getDefaultRenderCallback() { return iRenderAssetManager::getInstance()->m_default_render_callback; }
        static iRenderCallback* getForcedRenderCallback() { return iRenderAssetManager::getInstance()->m_forced_render_callback; }

    protected:
        std::unordered_map< std::string, iRenderAsset* > m_assets;
        iRenderCallback*                                 m_default_render_callback;
        iRenderCallback*                                 m_forced_render_callback;
    };

    template< typename T, typename Tasset >
    iRenderAssetManager< T, Tasset >::iRenderAssetManager()
    : m_default_render_callback( nullptr ),
      m_forced_render_callback( nullptr )
    {}

    template< typename T, typename Tasset >
    template< typename... Targs >
    Tasset* iRenderAssetManager< T, Tasset >::create( const std::string& _name, Targs... _args )
    {
        std::unordered_map< std::string, iRenderAsset* >& assets = iRenderAssetManager::getInstance()->m_assets;

        if( assets.contains( _name ) )
        {
            LOG_WARNING( std::format( "Camera already exist: {}", _name ) );
            return nullptr;
        }

        iRenderAsset* asset = MEMORY_ALLOC( T, 1, _args... );
        assets[ _name ]     = asset;

        LOG_MESSAGE( std::format( "Created asset: {}", _name ) );
        return asset;
    }

    template< typename T, typename Tasset >
    void iRenderAssetManager< T, Tasset >::update( const float& _delta_time )
    {
        std::unordered_map< std::string, iRenderAsset* >& assets = iRenderAssetManager::getInstance()->m_assets;

        for( iRenderAsset* asset : assets | std::views::values )
            asset->update( _delta_time );
    }

    template< typename T, typename Tasset >
    void iRenderAssetManager< T, Tasset >::render()
    {
        std::unordered_map< std::string, iRenderAsset* >& assets = iRenderAssetManager::getInstance()->m_assets;

        for( iRenderAsset* asset : assets | std::views::values )
            asset->render();
    }

    template< typename T, typename Tasset >
    bool iRenderAssetManager< T, Tasset >::add( const std::string& _name, Tasset* _asset )
    {
        std::unordered_map< std::string, iRenderAsset* >& assets = iRenderAssetManager::getInstance()->m_assets;

        if( assets.contains( _name ) )
        {
            LOG_WARNING( std::format( "Asset already exist: {}", _name ) );
            return false;
        }

        LOG_MESSAGE( std::format( "Added asset: {}", _name ) );
        assets[ _name ] = _asset;
        return true;
    }

    template< typename T, typename Tasset >
    bool iRenderAssetManager< T, Tasset >::destroy( const std::string& _name )
    {
        std::unordered_map< std::string, iRenderAsset* >& assets = iRenderAssetManager::getInstance()->m_assets;

        if( assets.erase( _name ) )
        {
            LOG_MESSAGE( std::format( "Destroyed asset: {}", _name ) );
            return true;
        }

        LOG_WARNING( std::format( "Asset doesn't exist: {}", _name ) );
        return false;
    }

    template< typename T, typename Tasset >
    bool iRenderAssetManager< T, Tasset >::destroy( const Tasset* _asset )
    {
        std::unordered_map< std::string, iRenderAsset* >& assets = iRenderAssetManager::getInstance()->m_assets;

        for( const std::pair< const std::string, iRenderAsset* >& asset : assets )
        {
            if( asset.second == _asset )
            {
                assets.erase( asset.first );
                LOG_MESSAGE( std::format( "Destroyed asset: {}", asset.first ) );
                return true;
            }
        }

        return false;
    }

    template< typename T, typename Tasset >
    void iRenderAssetManager< T, Tasset >::clear()
    {
        std::unordered_map< std::string, iRenderAsset* >& assets = iRenderAssetManager::getInstance()->m_assets;

        for( std::pair< const std::string, iRenderAsset* >& asset : assets )
        {
            LOG_MESSAGE( std::format( "Destroyed asset: {}", asset.first ) );
            MEMORY_FREE( asset.second );
        }

        assets.clear();
    }

    template< typename T, typename Tasset >
    Tasset* iRenderAssetManager< T, Tasset >::get( const std::string& _name )
    {
        std::unordered_map< std::string, iRenderAsset* >& assets = iRenderAssetManager::getInstance()->m_assets;

        const auto it = assets.find( _name );
        if( it == assets.end() )
        {
            LOG_WARNING( std::format( "Asset doesn't exist: {}", _name ) );
            return nullptr;
        }

        return reinterpret_cast< Tasset* >( it->second );
    }
}
