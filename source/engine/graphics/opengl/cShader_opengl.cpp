#include "cShader_opengl.h"

#include <fmt/format.h>
#include <glad/glad.h>
#include <slang-com-ptr.h>
#include <slang.h>
#include <spirv_cross/spirv_glsl.hpp>

#include "engine/core/cFileSystem.h"
#include "engine/core/Log.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::opengl
{
	cShader_opengl::cShader_opengl( std::string _name )
		: iShader( std::move( _name ) )
		, m_program( 0 )
	{
		DF_ProfilingScopeCpu;

		const unsigned vertex   = compileShader( fmt::format( "{}.vert", name ), GL_VERTEX_SHADER );
		const unsigned fragment = compileShader( fmt::format( "{}.frag", name ), GL_FRAGMENT_SHADER );

		m_program = glCreateProgram();
		glAttachShader( m_program, vertex );
		glAttachShader( m_program, fragment );
		glLinkProgram( m_program );

		int success;
		glGetProgramiv( m_program, GL_LINK_STATUS, &success );

		if( success )
			DF_LogMessage( fmt::format( "Successfully linked shader program: {}", name ) );
		else
		{
			char info_log[ 512 ];
			glGetProgramInfoLog( m_program, 512, nullptr, info_log );
			DF_LogError( fmt::format( "Failed to link shader program: {} - {}", name, info_log ) );
		}

		glDeleteShader( vertex );
		glDeleteShader( fragment );
	}

	cShader_opengl::~cShader_opengl()
	{
		DF_ProfilingScopeCpu;

		glDeleteProgram( m_program );
	}

	void cShader_opengl::use() const
	{
		DF_ProfilingScopeCpu;

		glUseProgram( m_program );
	}

	void cShader_opengl::setUniform1B( const std::string& _name, const bool _value ) const
	{
		DF_ProfilingScopeCpu;

		glUniform1i( glGetUniformLocation( m_program, _name.data() ), _value );
	}

	void cShader_opengl::setUniform1I( const std::string& _name, const int _value ) const
	{
		DF_ProfilingScopeCpu;

		glUniform1i( glGetUniformLocation( m_program, _name.data() ), _value );
	}

	void cShader_opengl::setUniform1F( const std::string& _name, const float _value ) const
	{
		DF_ProfilingScopeCpu;

		glUniform1f( glGetUniformLocation( m_program, _name.data() ), _value );
	}

	void cShader_opengl::setUniform4F( const std::string& _name, const cVector4f& _vector ) const
	{
		DF_ProfilingScopeCpu;

		glUniform4f( glGetUniformLocation( m_program, _name.data() ), _vector.x(), _vector.y(), _vector.z(), _vector.w() );
	}

	void cShader_opengl::setUniform4F( const std::string& _name, const cColor& _color ) const
	{
		DF_ProfilingScopeCpu;

		glUniform4f( glGetUniformLocation( m_program, _name.data() ), _color.r, _color.g, _color.b, _color.a );
	}

	void cShader_opengl::setUniformMatrix4F( const std::string& _name, const cMatrix4f& _matrix, const int _amount, const bool _transpose ) const
	{
		DF_ProfilingScopeCpu;

		glUniformMatrix4fv( glGetUniformLocation( m_program, _name.data() ), _amount, _transpose, _matrix.data() );
	}

	unsigned cShader_opengl::compileShader( const std::string& _name, const int _type )
	{
		DF_ProfilingScopeCpu;

		static Slang::ComPtr< slang::IGlobalSession > slang_global_session;
		if( !slang_global_session.get() )
			createGlobalSession( slang_global_session.writeRef() );

		const slang::TargetDesc target_desc{
			.format  = SLANG_SPIRV,
			.profile = slang_global_session->findProfile( "glsl_430" ),
			.flags   = 0,
		};

		std::vector< slang::CompilerOptionEntry > option_entries;

		const slang::SessionDesc session_desc{
			.targets                  = &target_desc,
			.targetCount              = 1,
			.defaultMatrixLayoutMode  = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR,
			.compilerOptionEntries    = option_entries.data(),
			.compilerOptionEntryCount = static_cast< uint32_t >( option_entries.size() ),
		};

		Slang::ComPtr< slang::ISession > session;
		slang_global_session->createSession( session_desc, session.writeRef() );

		Slang::ComPtr< slang::IBlob > diagnostic_blob;
		const std::string             path         = cFileSystem::getPath( _name + ".slang" );
		slang::IModule*               slang_module = session->loadModule( path.data(), diagnostic_blob.writeRef() );

		Slang::ComPtr< slang::IEntryPoint > entry_point;
		slang_module->findEntryPointByName( "main", entry_point.writeRef() );

		std::vector< slang::IComponentType* > component_types;
		component_types.push_back( slang_module );
		component_types.push_back( entry_point );

		Slang::ComPtr< slang::IComponentType > composed_program;
		session->createCompositeComponentType( component_types.data(), static_cast< SlangInt >( component_types.size() ), composed_program.writeRef(), diagnostic_blob.writeRef() );

		Slang::ComPtr< slang::IBlob > spirv_code;
		composed_program->getEntryPointCode( 0, 0, spirv_code.writeRef(), diagnostic_blob.writeRef() );

		const uint32_t* spirv_binary = static_cast< const uint32_t* >( spirv_code->getBufferPointer() );
		const size_t    spirv_size   = spirv_code->getBufferSize() / sizeof( uint32_t );

		spirv_cross::CompilerGLSL          shader( spirv_binary, spirv_size );
		spirv_cross::CompilerGLSL::Options options;
		options.version                 = 430;
		options.es                      = false;
		options.vulkan_semantics        = false;
		options.separate_shader_objects = true;
		shader.set_common_options( options );
		shader.build_combined_image_samplers();

		std::string shader_string = shader.compile();
		const char* shader_source = shader_string.data();

		const unsigned shader_id = glCreateShader( _type );
		glShaderSource( shader_id, 1, &shader_source, nullptr );
		glCompileShader( shader_id );

		int success;
		glGetShaderiv( shader_id, GL_COMPILE_STATUS, &success );

		if( success )
			DF_LogMessage( fmt::format( "Successfully compiled shader: {}", _name ) );
		else
		{
			char info_log[ 512 ];
			glGetShaderInfoLog( shader_id, 512, nullptr, info_log );
			DF_LogError( fmt::format( "Failed to compile shader: {} - {}", _name, info_log ) );
		}

		return shader_id;
	}
}
