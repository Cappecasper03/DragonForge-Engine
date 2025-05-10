#pragma once

#include <functional>

#include "engine/core/utils/Misc.h"
#include "engine/graphics/vulkan/pipeline/sPipelineCreateInfo_vulkan.h"
#include "iRenderCallback.h"

namespace df
{
	template< typename T, typename... Targs >
	class cRenderCallback final : public iRenderCallback
	{
	public:
		DF_DisableCopyAndMove( cRenderCallback );

		explicit cRenderCallback( std::string _name, const std::string& _shader_name, void( _callback )( const T*, Targs... ) );
		explicit cRenderCallback( std::string _name, const std::vector< std::string >& _shader_names, void( _callback )( const T*, Targs... ) );

		explicit cRenderCallback( std::string _name, const vulkan::sPipelineCreateInfo_vulkan& _pipeline, void( _callback )( const T*, Targs... ) );
		explicit cRenderCallback( std::string _name, const std::vector< vulkan::sPipelineCreateInfo_vulkan >& _pipelines, void( _callback )( const T*, Targs... ) );

		~cRenderCallback() override;

		void render( Targs... _args );

	protected:
		std::vector< T* >                           m_data;
		std::function< void( const T*, Targs... ) > m_callback;
	};
}

#include "cRenderCallback.inl"
