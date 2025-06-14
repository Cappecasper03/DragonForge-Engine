#include "Helper_vulkan.h"

#include <fmt/format.h>
#include <slang-com-ptr.h>
#include <slang.h>
#include <sstream>
#include <vector>
#include <vk_mem_alloc.hpp>

#include "engine/core/cFileSystem.h"
#include "engine/core/Log.h"
#include "engine/graphics/cRenderer.h"
#include "engine/graphics/vulkan/cGraphicsApi_vulkan.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::vulkan::helper
{
	namespace init
	{
		vk::CommandPoolCreateInfo commandPoolCreateInfo( const uint32_t _queue_family_index, const vk::CommandPoolCreateFlags _flags )
		{
			DF_ProfilingScopeCpu;

			const vk::CommandPoolCreateInfo create_info( _flags, _queue_family_index );
			return create_info;
		}

		vk::CommandBufferBeginInfo commandBufferBeginInfo( const vk::CommandBufferUsageFlags _flags )
		{
			DF_ProfilingScopeCpu;

			const vk::CommandBufferBeginInfo begin_info( _flags );
			return begin_info;
		}

		vk::FenceCreateInfo fenceCreateInfo( const vk::FenceCreateFlags _flags )
		{
			DF_ProfilingScopeCpu;

			const vk::FenceCreateInfo create_info( _flags );
			return create_info;
		}
		vk::SemaphoreCreateInfo semaphoreCreateInfo()
		{
			DF_ProfilingScopeCpu;

			return vk::SemaphoreCreateInfo();
		}

		vk::SemaphoreSubmitInfo semaphoreSubmitInfo( const vk::PipelineStageFlags2 _stage_mask, const vk::Semaphore& _semaphore )
		{
			DF_ProfilingScopeCpu;

			const vk::SemaphoreSubmitInfo submit_info( _semaphore, static_cast< uint32_t >( 1 ), _stage_mask, 0 );
			return submit_info;
		}

		vk::CommandBufferSubmitInfo commandBufferSubmitInfo( const vk::CommandBuffer& _command_buffer )
		{
			DF_ProfilingScopeCpu;

			const vk::CommandBufferSubmitInfo submit_info( _command_buffer, 0 );
			return submit_info;
		}

		vk::SubmitInfo2 submitInfo( const vk::CommandBufferSubmitInfo* _command_buffer,
		                            const vk::SemaphoreSubmitInfo*     _signal_semaphore_info,
		                            const vk::SemaphoreSubmitInfo*     _wait_semaphore_info )
		{
			DF_ProfilingScopeCpu;

			const uint32_t wait_count    = _wait_semaphore_info ? 1 : 0;
			const uint32_t command_count = _command_buffer ? 1 : 0;
			const uint32_t signal_count  = _signal_semaphore_info ? 1 : 0;

			const vk::SubmitInfo2 submit_info( vk::SubmitFlags(), wait_count, _wait_semaphore_info, command_count, _command_buffer, signal_count, _signal_semaphore_info );
			return submit_info;
		}

		vk::SubmitInfo2 submitInfo( const std::vector< vk::CommandBufferSubmitInfo >& _command_buffers,
		                            const std::vector< vk::SemaphoreSubmitInfo >&     _signal_semaphore_infos,
		                            const std::vector< vk::SemaphoreSubmitInfo >&     _wait_semaphore_infos )
		{
			DF_ProfilingScopeCpu;

			const vk::SubmitInfo2 submit_info( vk::SubmitFlags(), _wait_semaphore_infos, _command_buffers, _signal_semaphore_infos );
			return submit_info;
		}

		vk::PresentInfoKHR presentInfo( const vk::Semaphore* _semaphore, const vk::SwapchainKHR* _swapchain, const uint32_t* _swap_chain_index )
		{
			DF_ProfilingScopeCpu;

			const uint32_t semaphore_count = _semaphore ? 1 : 0;
			const uint32_t swapchain_count = _swapchain ? 1 : 0;

			const vk::PresentInfoKHR present_info( semaphore_count, _semaphore, swapchain_count, _swapchain, _swap_chain_index );
			return present_info;
		}

		vk::PresentInfoKHR presentInfo( const std::vector< vk::Semaphore >&    _semaphores,
		                                const std::vector< vk::SwapchainKHR >& _swapchains,
		                                const std::vector< uint32_t >&         _swap_chain_indices )
		{
			DF_ProfilingScopeCpu;

			const vk::PresentInfoKHR present_info( _semaphores, _swapchains, _swap_chain_indices );
			return present_info;
		}

		vk::RenderingAttachmentInfo attachmentInfo( const vk::ImageView& _view, const vk::ClearValue* _clear, const vk::ImageLayout _layout )
		{
			DF_ProfilingScopeCpu;

			const vk::RenderingAttachmentInfo attachment_info( _view,
			                                                   _layout,
			                                                   vk::ResolveModeFlagBits::eNone,
			                                                   nullptr,
			                                                   vk::ImageLayout::eUndefined,
			                                                   _clear ? vk::AttachmentLoadOp::eClear : vk::AttachmentLoadOp::eLoad,
			                                                   vk::AttachmentStoreOp::eStore,
			                                                   _clear ? *_clear : vk::ClearValue{} );
			return attachment_info;
		}

		vk::RenderingAttachmentInfo depthAttachmentInfo( const vk::ImageView& _view, const vk::ImageLayout _layout )
		{
			DF_ProfilingScopeCpu;

			const vk::RenderingAttachmentInfo attachment_info( _view,
			                                                   _layout,
			                                                   vk::ResolveModeFlagBits::eNone,
			                                                   nullptr,
			                                                   vk::ImageLayout::eUndefined,
			                                                   vk::AttachmentLoadOp::eClear,
			                                                   vk::AttachmentStoreOp::eStore,
			                                                   vk::ClearValue( vk::ClearDepthStencilValue( 0 ) ) );
			return attachment_info;
		}

		vk::PipelineShaderStageCreateInfo pipelineShaderStageCreateInfo( const vk::ShaderStageFlagBits _stage, const vk::ShaderModule& _module )
		{
			DF_ProfilingScopeCpu;

			const vk::PipelineShaderStageCreateInfo info( vk::PipelineShaderStageCreateFlags(), _stage, _module, "main" );
			return info;
		}
	}

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
