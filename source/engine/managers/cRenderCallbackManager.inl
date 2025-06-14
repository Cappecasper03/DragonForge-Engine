#pragma once

#include "cRenderCallbackManager.h"

#include <unordered_map>

#include "engine/graphics/callback/cRenderCallback.h"
#include "engine/graphics/vulkan/pipeline/cPipelineCreateInfo_vulkan.h"
#include "engine/managers/assets/cModelManager.h"

#include <fmt/format.h>

#include "engine/core/utils/iSingleton.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	template< typename T, typename... Targs >
	iRenderCallback* cRenderCallbackManager::create( const std::string& _shader_name, void _callback( const T*, Targs... ) )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, cUnique< iRenderCallback > >& render_callbacks = getInstance()->m_render_callbacks;

		cUnique< iRenderCallback >& callback = render_callbacks[ _shader_name ];

		if( callback )
		{
			DF_LogWarning( fmt::format( "Callback already exist: {}", _shader_name ) );
			return nullptr;
		}

		callback = MakeUnique< cRenderCallback< T, Targs... > >( _shader_name, _shader_name, _callback );

		DF_LogMessage( fmt::format( "Created callback: {}", _shader_name ) );
		return callback.get();
	}

	template< typename T, typename... Targs >
	iRenderCallback* cRenderCallbackManager::create( const std::string& _name, const vulkan::cPipelineCreateInfo_vulkan& _pipeline, void _callback( const T*, Targs... ) )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, cUnique< iRenderCallback > >& render_callbacks = getInstance()->m_render_callbacks;

		cUnique< iRenderCallback >& callback = render_callbacks[ _name ];

		if( callback )
		{
			DF_LogWarning( fmt::format( "Callback already exist: {}", _name ) );
			return nullptr;
		}

		callback = MakeUnique< cRenderCallback< T, Targs... > >( _name, _pipeline, _callback );

		DF_LogMessage( fmt::format( "Created callback: {}", _name ) );
		return callback.get();
	}

	inline bool cRenderCallbackManager::destroy( const std::string& _name )
	{
		DF_ProfilingScopeCpu;

		std::unordered_map< std::string, cUnique< iRenderCallback > >& render_callbacks = getInstance()->m_render_callbacks;

		const auto it = render_callbacks.find( _name );
		if( it == render_callbacks.end() )
		{
			DF_LogWarning( fmt::format( "Callback doesn't exist: {}", _name ) );
			return false;
		}

		render_callbacks.erase( it );
		DF_LogMessage( fmt::format( "Destroyed callback: {}", _name ) );

		return true;
	}

	inline bool cRenderCallbackManager::destroy( const iRenderCallback* _callback )
	{
		DF_ProfilingScopeCpu;

		if( !_callback )
			return false;

		std::unordered_map< std::string, cUnique< iRenderCallback > >& render_callbacks = getInstance()->m_render_callbacks;

		for( const std::pair< const std::string, cUnique< iRenderCallback > >& callback: render_callbacks )
		{
			if( callback.second.get() == _callback )
			{
				DF_LogMessage( fmt::format( "Destroyed callback: {}", callback.first ) );
				render_callbacks.erase( callback.first );
				return true;
			}
		}

		DF_LogWarning( fmt::format( "Callback isn't managed: {}", _callback->m_name ) );
		return false;
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

		std::unordered_map< std::string, cUnique< iRenderCallback > >& render_callbacks = getInstance()->m_render_callbacks;

		const auto it = render_callbacks.find( _name );
		if( it == render_callbacks.end() )
		{
			DF_LogWarning( fmt::format( "Callback doesn't exist: {}", _name ) );
			return nullptr;
		}

		return it->second.get();
	}
}
