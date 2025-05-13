#pragma once

#include <filesystem>
#include <fmt/format.h>
#include <ranges>

#include "engine/core/cFileSystem.h"
#include "engine/core/Log.h"
#include "engine/graphics/assets/iObject.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	template< typename T, typename Tasset >
	iObjectManager< T, Tasset >::~iObjectManager()
	{
		DF_ProfilingScopeCpu;

		clear();
	}

	template< typename T, typename Tasset >
	template< typename Ttype, typename... Targs >
	Tasset* iObjectManager< T, Tasset >::create( const std::string& _name, Targs... _args )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, iObject* >& assets = iObjectManager::getInstance()->m_objects;

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
	bool iObjectManager< T, Tasset >::add( Tasset* _asset )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, iObject* >& assets = iObjectManager::getInstance()->m_objects;

		if( assets.contains( _asset->m_name ) )
		{
			DF_LogWarning( fmt::format( "Asset already exist: {}", _asset->m_name ) );
			return false;
		}

		assets[ _asset->m_name ] = _asset;

		DF_LogMessage( fmt::format( "Added Asset: {}", _asset->m_name ) );
		return true;
	}

	template< typename T, typename Tasset >
	void iObjectManager< T, Tasset >::update( const float _delta_time )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, iObject* >& assets = iObjectManager::getInstance()->m_objects;

		for( iObject* asset: assets | std::views::values )
			asset->update( _delta_time );
	}

	template< typename T, typename Tasset >
	bool iObjectManager< T, Tasset >::destroy( const std::string& _name )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, iObject* >& assets = iObjectManager::getInstance()->m_objects;

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
	bool iObjectManager< T, Tasset >::destroy( const Tasset* _asset )
	{
		DF_ProfilingScopeCpu;

		if( !_asset )
			return false;

		std::unordered_map< std::string, iObject* >& assets = iObjectManager::getInstance()->m_objects;

		for( const std::pair< const std::string, iObject* >& asset: assets )
		{
			if( asset.second == _asset )
			{
				DF_LogMessage( fmt::format( "Destroyed asset: {}", asset.first ) );
				delete asset.second;
				assets.erase( asset.first );
				return true;
			}
		}

		DF_LogWarning( fmt::format( "Asset isn't managed: {}", _asset->m_name ) );
		return false;
	}

	template< typename T, typename Tasset >
	void iObjectManager< T, Tasset >::clear()
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, iObject* >& assets = iObjectManager::getInstance()->m_objects;

		for( std::pair< const std::string, iObject* >& asset: assets )
		{
			DF_LogMessage( fmt::format( "Destroyed asset: {}", asset.first ) );
			delete asset.second;
		}

		assets.clear();
	}

	template< typename T, typename Tasset >
	Tasset* iObjectManager< T, Tasset >::get( const std::string& _name )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, iObject* >& assets = iObjectManager::getInstance()->m_objects;

		const auto it = assets.find( _name );
		if( it == assets.end() )
		{
			DF_LogWarning( fmt::format( "Asset doesn't exist: {}", _name ) );
			return nullptr;
		}

		return reinterpret_cast< Tasset* >( it->second );
	}
}
