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
    class iAssetManager : public iSingleton< T >
    {
    public:
        DISABLE_COPY_AND_MOVE( iAssetManager );

        iAssetManager();
        ~iAssetManager() override;

        template< typename... Targs >
        static Tasset* create( const std::string& _name, Targs... _args );

        static void update( const float& _delta_time );
        static void render();

        static bool destroy( const std::string& _name );
        static bool destroy( const Tasset* _asset );
        static void clear();

        static Tasset* get( const std::string& _name );

        static iRenderCallback* getDefaultRenderCallback() { return iAssetManager::getInstance()->m_default_render_callback; }
        static iRenderCallback* getForcedRenderCallback() { return iAssetManager::getInstance()->m_forced_render_callback; }

    protected:
        std::unordered_map< std::string, iAsset* > m_assets;
        iRenderCallback*                           m_default_render_callback;
        iRenderCallback*                           m_forced_render_callback;
    };

    template< typename T, typename Tasset >
    iAssetManager< T, Tasset >::iAssetManager()
    : m_default_render_callback( nullptr ),
      m_forced_render_callback( nullptr )
    {}

    template< typename T, typename Tasset >
    iAssetManager< T, Tasset >::~iAssetManager()
    {
        clear();
    }

    template< typename T, typename Tasset >
    template< typename... Targs >
    Tasset* iAssetManager< T, Tasset >::create( const std::string& _name, Targs... _args )
    {
        std::unordered_map< std::string, iAsset* >& assets = iAssetManager::getInstance()->m_assets;

        if( assets.contains( _name ) )
        {
            LOG_WARNING( std::format( "Camera already exist: {}", _name ) );
            return nullptr;
        }

        Tasset* asset   = MEMORY_ALLOC( Tasset, 1, _name, _args... );
        assets[ _name ] = asset;

        LOG_MESSAGE( std::format( "Created asset: {}", _name ) );
        return asset;
    }

    template< typename T, typename Tasset >
    void iAssetManager< T, Tasset >::update( const float& _delta_time )
    {
        std::unordered_map< std::string, iAsset* >& assets = iAssetManager::getInstance()->m_assets;

        for( iAsset* asset : assets | std::views::values )
            asset->update( _delta_time );
    }

    template< typename T, typename Tasset >
    void iAssetManager< T, Tasset >::render()
    {
        std::unordered_map< std::string, iAsset* >& assets = iAssetManager::getInstance()->m_assets;

        for( iAsset* asset : assets | std::views::values )
            asset->render();
    }

    template< typename T, typename Tasset >
    bool iAssetManager< T, Tasset >::destroy( const std::string& _name )
    {
        std::unordered_map< std::string, iAsset* >& assets = iAssetManager::getInstance()->m_assets;

        const auto it = assets.find( _name );
        if( it == assets.end() )
        {
            LOG_WARNING( std::format( "Asset doesn't exist: {}", _name ) );
            return false;
        }

        MEMORY_FREE( it->second );
        assets.erase( it );
        LOG_MESSAGE( std::format( "Destroyed asset: {}", _name ) );

        return true;
    }

    template< typename T, typename Tasset >
    bool iAssetManager< T, Tasset >::destroy( const Tasset* _asset )
    {
        std::unordered_map< std::string, iAsset* >& assets = iAssetManager::getInstance()->m_assets;

        for( const std::pair< const std::string, iAsset* >& asset : assets )
        {
            if( asset.second == _asset )
            {
                LOG_MESSAGE( std::format( "Destroyed asset: {}", asset.first ) );
                MEMORY_FREE( asset.second );
                assets.erase( asset.first );
                return true;
            }
        }

        return false;
    }

    template< typename T, typename Tasset >
    void iAssetManager< T, Tasset >::clear()
    {
        std::unordered_map< std::string, iAsset* >& assets = iAssetManager::getInstance()->m_assets;

        for( std::pair< const std::string, iAsset* >& asset : assets )
        {
            LOG_MESSAGE( std::format( "Destroyed asset: {}", asset.first ) );
            MEMORY_FREE( asset.second );
        }

        assets.clear();
    }

    template< typename T, typename Tasset >
    Tasset* iAssetManager< T, Tasset >::get( const std::string& _name )
    {
        std::unordered_map< std::string, iAsset* >& assets = iAssetManager::getInstance()->m_assets;

        const auto it = assets.find( _name );
        if( it == assets.end() )
        {
            LOG_WARNING( std::format( "Asset doesn't exist: {}", _name ) );
            return nullptr;
        }

        return reinterpret_cast< Tasset* >( it->second );
    }
}
