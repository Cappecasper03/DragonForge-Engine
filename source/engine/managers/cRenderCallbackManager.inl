#pragma once

#include <fmt/format.h>

#include "engine/misc/iSingleton.h"
#include "engine/profiling/ProfilingMacros.h"
#include "engine/rendering/callback/cRenderCallback.h"
#include "engine/rendering/vulkan/pipeline/sPipelineCreateInfo_vulkan.h"

namespace df
{
	template< typename T, typename... Targs >
	iRenderCallback* cRenderCallbackManager::create( const std::string& _shader_name, void _callback( const T*, Targs... ) )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

		if( render_callbacks.contains( _shader_name ) )
		{
			DF_LogWarning( fmt::format( "Callback already exist: {}", _shader_name ) );
			return nullptr;
		}

		cRenderCallback< T, Targs... >* callback = new cRenderCallback< T, Targs... >( _shader_name, _shader_name, _callback );
		render_callbacks[ _shader_name ]         = callback;

		DF_LogMessage( fmt::format( "Created callback: {}", _shader_name ) );
		return callback;
	}

	template< typename T, typename... Targs >
	iRenderCallback* cRenderCallbackManager::create( const std::string& _callback_name, const std::vector< std::string >& _shader_names, void _callback( const T*, Targs... ) )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

		if( render_callbacks.contains( _callback_name ) )
		{
			DF_LogWarning( fmt::format( "Callback already exist: {}", _callback_name ) );
			return nullptr;
		}

		cRenderCallback< T, Targs... >* callback = new cRenderCallback< T, Targs... >( _callback_name, _shader_names, _callback );
		render_callbacks[ _callback_name ]       = callback;

		DF_LogMessage( fmt::format( "Created callback: {}", _callback_name ) );
		return callback;
	}

	template< typename T, typename... Targs >
	iRenderCallback* cRenderCallbackManager::create( const std::string& _name, const vulkan::sPipelineCreateInfo_vulkan& _pipelines, void _callback( const T*, Targs... ) )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

		if( render_callbacks.contains( _name ) )
		{
			DF_LogWarning( fmt::format( "Callback already exist: {}", _name ) );
			return nullptr;
		}

		cRenderCallback< T, Targs... >* callback = new cRenderCallback< T, Targs... >( _name, _pipelines, _callback );
		render_callbacks[ _name ]                = callback;

		DF_LogMessage( fmt::format( "Created callback: {}", _name ) );
		return callback;
	}

	template< typename T, typename... Targs >
	iRenderCallback* cRenderCallbackManager::create( const std::string&                                       _name,
	                                                 const std::vector< vulkan::sPipelineCreateInfo_vulkan >& _pipelines,
	                                                 void                                                     _callback( const T*, Targs... ) )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

		if( render_callbacks.contains( _name ) )
		{
			DF_LogWarning( fmt::format( "Callback already exist: {}", _name ) );
			return nullptr;
		}

		cRenderCallback< T, Targs... >* callback = new cRenderCallback< T, Targs... >( _name, _pipelines, _callback );
		render_callbacks[ _name ]                = callback;

		DF_LogMessage( fmt::format( "Created callback: {}", _name ) );
		return callback;
	}

	inline bool cRenderCallbackManager::destroy( const std::string& _name )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

		const auto it = render_callbacks.find( _name );
		if( it == render_callbacks.end() )
		{
			DF_LogWarning( fmt::format( "Callback doesn't exist: {}", _name ) );
			return false;
		}

		delete it->second;
		render_callbacks.erase( it );
		DF_LogMessage( fmt::format( "Destroyed callback: {}", _name ) );

		return true;
	}

	inline bool cRenderCallbackManager::destroy( const iRenderCallback* _callback )
	{
		DF_ProfilingScopeCpu;

		if( !_callback )
			return false;

		std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

		for( const std::pair< const std::string, iRenderCallback* >& callback: render_callbacks )
		{
			if( callback.second == _callback )
			{
				DF_LogMessage( fmt::format( "Destroyed callback: {}", callback.first ) );
				delete callback.second;
				render_callbacks.erase( callback.first );
				return true;
			}
		}

		DF_LogWarning( fmt::format( "Callback isn't managed: {}", _callback->name ) );
		return false;
	}

	inline void cRenderCallbackManager::clear()
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

		for( const std::pair< const std::string, iRenderCallback* >& callback: render_callbacks )
		{
			if( callback.second )
			{
				DF_LogMessage( fmt::format( "Destroyed callback: {}", callback.first ) );
				delete callback.second;
			}
		}

		render_callbacks.clear();
	}

	template< typename T, typename... Targs >
	void cRenderCallbackManager::render( const std::string& _name, Targs... _args )
	{
		DF_ProfilingScopeCpu;

		cRenderCallback< T, Targs... >* callback = reinterpret_cast< cRenderCallback< T, Targs... >* >( getInstance()->m_render_callbacks[ _name ] );
		if( callback )
			callback->render( _args... );
	}

	template< typename T, typename... Targs >
	void cRenderCallbackManager::render( iRenderCallback* _callback, Targs... _args )
	{
		DF_ProfilingScopeCpu;

		cRenderCallback< T, Targs... >* callback = reinterpret_cast< cRenderCallback< T, Targs... >* >( _callback );
		if( callback )
			callback->render( _args... );
	}

	inline iRenderCallback* cRenderCallbackManager::get( const std::string& _name )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, iRenderCallback* >& render_callbacks = getInstance()->m_render_callbacks;

		const auto it = render_callbacks.find( _name );
		if( it == render_callbacks.end() )
		{
			DF_LogWarning( fmt::format( "Callback doesn't exist: {}", _name ) );
			return nullptr;
		}

		return it->second;
	}
}
