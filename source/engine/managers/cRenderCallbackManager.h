#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "engine/core/utils/iSingleton.h"
#include "engine/graphics/vulkan/pipeline/cPipelineCreateInfo_vulkan.h"

namespace df
{
	struct iRenderCallback;

	class cRenderCallbackManager : public iSingleton< cRenderCallbackManager >
	{
	public:
		DF_DeleteCopyAndMove( cRenderCallbackManager );

		cRenderCallbackManager() = default;
		~cRenderCallbackManager() override { clear(); }

		template< typename T, typename... Targs >
		static iRenderCallback* create( const std::string& _shader_name, void( _callback )( const T*, Targs... ) );
		template< typename T, typename... Targs >
		static iRenderCallback* create( const std::string& _callback_name, const std::vector< std::string >& _shader_names, void( _callback )( const T*, Targs... ) );

		template< typename T, typename... Targs >
		static iRenderCallback* create( const std::string& _name, const vulkan::cPipelineCreateInfo_vulkan& _pipelines, void( _callback )( const T*, Targs... ) );
		template< typename T, typename... Targs >
		static iRenderCallback* create( const std::string& _name, const std::vector< vulkan::cPipelineCreateInfo_vulkan >& _pipelines, void( _callback )( const T*, Targs... ) );

		static bool destroy( const std::string& _name );
		static bool destroy( const iRenderCallback* _callback );
		static void clear();

		template< typename T, typename... Targs >
		static void render( const std::string& _name, Targs... _args );
		template< typename T, typename... Targs >
		static void render( iRenderCallback* _callback, Targs... _args );

		static iRenderCallback* get( const std::string& _name );

	private:
		std::unordered_map< std::string, iRenderCallback* > m_render_callbacks;
	};
}

#include "cRenderCallbackManager.inl"
