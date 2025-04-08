#include <fmt/format.h>
#include <ranges>
#include <string>
#include <unordered_map>

#include "engine/profiling/ProfilingMacros.h"
#include "engine/log/Log.h"
#include "engine/rendering/assets/AssetTypes.h"

namespace df
{
	template< typename T, typename Tasset >
	iAssetManager< T, Tasset >::iAssetManager()
		: m_default_render_callback( nullptr )
		, m_forced_render_callback( nullptr )
	{}

	template< typename T, typename Tasset >
	iAssetManager< T, Tasset >::~iAssetManager()
	{
		DF_ProfilingScopeCPU;

		clear();
	}

	template< typename T, typename Tasset >
	template< typename Ttype, typename... Targs >
	Tasset* iAssetManager< T, Tasset >::create( const std::string& _name, Targs... _args )
	{
		DF_ProfilingScopeCPU;

		std::unordered_map< std::string, iAsset* >& assets = iAssetManager::getInstance()->m_assets;

		if( assets.contains( _name ) )
		{
			DF_LogWarning( fmt::format( "Asset already exist: {}", _name ) );
			return nullptr;
		}

		Ttype* asset    = new Ttype( _name, _args... );
		assets[ _name ] = asset;

		DF_LogMessage( fmt::format( "Created asset: {}", _name ) );
		return asset;
	}

	template< typename T, typename Tasset >
	bool iAssetManager< T, Tasset >::add( Tasset* _asset )
	{
		DF_ProfilingScopeCPU;

		std::unordered_map< std::string, iAsset* >& assets = iAssetManager::getInstance()->m_assets;

		if( assets.contains( _asset->name ) )
		{
			DF_LogWarning( fmt::format( "Asset already exist: {}", _asset->name ) );
			return false;
		}

		assets[ _asset->name ] = _asset;

		DF_LogMessage( fmt::format( "Added Asset: {}", _asset->name ) );
		return true;
	}

	template< typename T, typename Tasset >
	void iAssetManager< T, Tasset >::update( const float _delta_time )
	{
		DF_ProfilingScopeCPU;

		std::unordered_map< std::string, iAsset* >& assets = iAssetManager::getInstance()->m_assets;

		for( iAsset* asset: assets | std::views::values )
			asset->update( _delta_time );
	}

	template< typename T, typename Tasset >
	void iAssetManager< T, Tasset >::render()
	{
		DF_ProfilingScopeCPU;

		std::unordered_map< std::string, iAsset* >& assets = iAssetManager::getInstance()->m_assets;

		for( iAsset* asset: assets | std::views::values )
			asset->render();
	}

	template< typename T, typename Tasset >
	bool iAssetManager< T, Tasset >::destroy( const std::string& _name )
	{
		DF_ProfilingScopeCPU;

		std::unordered_map< std::string, iAsset* >& assets = iAssetManager::getInstance()->m_assets;

		const auto it = assets.find( _name );
		if( it == assets.end() )
		{
			DF_LogWarning( fmt::format( "Asset doesn't exist: {}", _name ) );
			return false;
		}

		delete it->second;
		assets.erase( it );
		DF_LogMessage( fmt::format( "Destroyed asset: {}", _name ) );

		return true;
	}

	template< typename T, typename Tasset >
	bool iAssetManager< T, Tasset >::destroy( const Tasset* _asset )
	{
		DF_ProfilingScopeCPU;

		if( !_asset )
			return false;

		std::unordered_map< std::string, iAsset* >& assets = iAssetManager::getInstance()->m_assets;

		for( const std::pair< const std::string, iAsset* >& asset: assets )
		{
			if( asset.second == _asset )
			{
				DF_LogMessage( fmt::format( "Destroyed asset: {}", asset.first ) );
				delete asset.second;
				assets.erase( asset.first );
				return true;
			}
		}

		DF_LogWarning( fmt::format( "Asset isn't managed: {}", _asset->name ) );
		return false;
	}

	template< typename T, typename Tasset >
	void iAssetManager< T, Tasset >::clear()
	{
		DF_ProfilingScopeCPU;

		std::unordered_map< std::string, iAsset* >& assets = iAssetManager::getInstance()->m_assets;

		for( std::pair< const std::string, iAsset* >& asset: assets )
		{
			DF_LogMessage( fmt::format( "Destroyed asset: {}", asset.first ) );
			delete asset.second;
		}

		assets.clear();
	}

	template< typename T, typename Tasset >
	Tasset* iAssetManager< T, Tasset >::get( const std::string& _name )
	{
		DF_ProfilingScopeCPU;

		std::unordered_map< std::string, iAsset* >& assets = iAssetManager::getInstance()->m_assets;

		const auto it = assets.find( _name );
		if( it == assets.end() )
		{
			DF_LogWarning( fmt::format( "Asset doesn't exist: {}", _name ) );
			return nullptr;
		}

		return reinterpret_cast< Tasset* >( it->second );
	}
}
