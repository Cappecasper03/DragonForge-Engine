#include "Helper_vulkan.h"

#include <fmt/format.h>
#include <slang-com-ptr.h>
#include <slang.h>
#include <sstream>
#include <vector>

#include "engine/core/cFileSystem.h"
#include "engine/core/Log.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cGraphicsApi_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan::helper
{
	namespace util
	{
		void transitionImage( const vk::CommandBuffer& _command_buffer, const vk::Image& _image, const vk::ImageLayout _current_layout, const vk::ImageLayout _new_layout )
		{
			DF_ProfilingScopeCpu;

			const vk::ImageSubresourceRange subresource_range( _new_layout == vk::ImageLayout::eDepthAttachmentOptimal ? vk::ImageAspectFlagBits::eDepth
			                                                                                                           : vk::ImageAspectFlagBits::eColor,
			                                                   0,
			                                                   vk::RemainingMipLevels,
			                                                   0,
			                                                   vk::RemainingArrayLayers );

			const vk::ImageMemoryBarrier2 memory_barrier( vk::PipelineStageFlagBits2::eAllCommands,
			                                              vk::AccessFlagBits2::eMemoryWrite,
			                                              vk::PipelineStageFlagBits2::eAllCommands,
			                                              vk::AccessFlagBits2::eMemoryWrite | vk::AccessFlagBits2::eMemoryRead,
			                                              _current_layout,
			                                              _new_layout,
			                                              0,
			                                              0,
			                                              _image,
			                                              subresource_range );

			const vk::DependencyInfo info( vk::DependencyFlags(), 0, nullptr, 0, nullptr, 1, &memory_barrier );
			_command_buffer.pipelineBarrier2( info );
		}

		void copyImageToImage( const vk::CommandBuffer& _command_buffer,
		                       const vk::Image&         _source,
		                       const vk::Image&         _destination,
		                       const vk::Extent2D       _source_size,
		                       const vk::Extent2D       _destination_size )
		{
			DF_ProfilingScopeCpu;

			const std::array< vk::Offset3D, 2 > source{
				{
                 vk::Offset3D(),
                 vk::Offset3D( static_cast< int32_t >( _source_size.width ), static_cast< int32_t >( _source_size.height ), 1 ),
				 }
			};
			const std::array< vk::Offset3D, 2 > destination{
				{
                 vk::Offset3D(),
                 vk::Offset3D( static_cast< int32_t >( _destination_size.width ), static_cast< int32_t >( _destination_size.height ), 1 ),
				 }
			};

			const vk::ImageBlit2 blit_region( vk::ImageSubresourceLayers( vk::ImageAspectFlagBits::eColor, 0, 0, 1 ),
			                                  source,
			                                  vk::ImageSubresourceLayers( vk::ImageAspectFlagBits::eColor, 0, 0, 1 ),
			                                  destination );

			const vk::BlitImageInfo2
				blit_info( _source, vk::ImageLayout::eTransferSrcOptimal, _destination, vk::ImageLayout::eTransferDstOptimal, 1, &blit_region, vk::Filter::eLinear );
			_command_buffer.blitImage2( blit_info );
		}

		vk::ShaderModule createShaderModule( const std::string& _name )
		{
			DF_ProfilingScopeCpu;

			std::string       slang_shader_path = cFileSystem::getPath( _name + ".slang" );
			std::ifstream     inputFile( slang_shader_path );
			std::stringstream buffer;
			buffer << inputFile.rdbuf();
			std::string originalContent = buffer.str();
			inputFile.close();
			std::string slang_shader_source = "#define DF_Vulkan\n" + originalContent;

			static Slang::ComPtr< slang::IGlobalSession > slang_global_session;
			if( !slang_global_session.get() )
				createGlobalSession( slang_global_session.writeRef() );

			const slang::TargetDesc target_desc{
				.format  = SLANG_SPIRV,
				.profile = slang_global_session->findProfile( "glsl_450" ),
				.flags   = 0,
			};

			const slang::SessionDesc session_desc{
				.targets                 = &target_desc,
				.targetCount             = 1,
				.defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR,
			};

			Slang::ComPtr< slang::ISession > session;
			slang_global_session->createSession( session_desc, session.writeRef() );

			Slang::ComPtr< slang::IBlob > diagnostic_blob;
			slang::IModule* slang_module = session->loadModuleFromSourceString( _name.data(), slang_shader_path.data(), slang_shader_source.data(), diagnostic_blob.writeRef() );

			Slang::ComPtr< slang::IEntryPoint > entry_point;
			slang_module->findEntryPointByName( "main", entry_point.writeRef() );

			std::vector< slang::IComponentType* > component_types;
			component_types.push_back( slang_module );
			component_types.push_back( entry_point );

			Slang::ComPtr< slang::IComponentType > composed_program;
			session->createCompositeComponentType( component_types.data(),
			                                       static_cast< SlangInt >( component_types.size() ),
			                                       composed_program.writeRef(),
			                                       diagnostic_blob.writeRef() );

			Slang::ComPtr< slang::IBlob > spirv_code;
			composed_program->getEntryPointCode( 0, 0, spirv_code.writeRef(), diagnostic_blob.writeRef() );

			const vk::ShaderModuleCreateInfo create_info( vk::ShaderModuleCreateFlags(),
			                                              spirv_code->getBufferSize(),
			                                              static_cast< const uint32_t* >( spirv_code->getBufferPointer() ) );

			const cGraphicsApi_vulkan* graphics_api = reinterpret_cast< cGraphicsApi_vulkan* >( cRenderer::getApi() );

			const vk::ShaderModule module = graphics_api->getLogicalDevice().createShaderModule( create_info ).value;
			DF_LogMessage( fmt::format( "Successfully loaded shader and created shader module: {}", _name ) );
			return module;
		}
	}
}
