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
