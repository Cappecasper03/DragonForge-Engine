#include "cShader_opengl.h"

#include <fmt/format.h>
#include <glad/glad.h>
#include <iostream>
#include <slang-com-ptr.h>
#include <slang.h>
#include <spirv_cross/spirv_glsl.hpp>
#include <sstream>

#include "engine/core/cFileSystem.h"
#include "engine/core/Log.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df::opengl
{
	cShader_opengl::cShader_opengl()
		: m_program( 0 )
	{}

	cShader_opengl::cShader_opengl( const std::string& _name )
		: m_program( 0 )
		, m_name( _name )
	{
		DF_ProfilingScopeCpu;

		const unsigned vertex   = compileShader( fmt::format( "{}.vert", m_name ), GL_VERTEX_SHADER );
		const unsigned fragment = compileShader( fmt::format( "{}.frag", m_name ), GL_FRAGMENT_SHADER );

		createProgram( vertex, fragment );
	}

	cShader_opengl::~cShader_opengl()
	{
		DF_ProfilingScopeCpu;

		glDeleteProgram( m_program );
	}

	void cShader_opengl::load( const std::string& _name )
	{
		DF_ProfilingScopeCpu;

		m_name = _name;

		const unsigned vertex   = compileShader( fmt::format( "{}.vert", m_name ), GL_VERTEX_SHADER );
		const unsigned fragment = compileShader( fmt::format( "{}.frag", m_name ), GL_FRAGMENT_SHADER );

		createProgram( vertex, fragment );
	}

	void cShader_opengl::use() const
	{
		DF_ProfilingScopeCpu;

		glUseProgram( m_program );
	}

	void cShader_opengl::setBool( const std::string& _name, const bool _value ) const
	{
		DF_ProfilingScopeCpu;

		glUniform1i( glGetUniformLocation( m_program, _name.data() ), _value );
	}

	void cShader_opengl::setUnsignedInt( const std::string& _name, const unsigned _value ) const
	{
		DF_ProfilingScopeCpu;

		glUniform1ui( glGetUniformLocation( m_program, _name.data() ), _value );
	}

	void cShader_opengl::setInt( const std::string& _name, const int _value ) const
	{
		DF_ProfilingScopeCpu;

		glUniform1i( glGetUniformLocation( m_program, _name.data() ), _value );
	}

	void cShader_opengl::setFloat( const std::string& _name, const float _value ) const
	{
		DF_ProfilingScopeCpu;

		glUniform1f( glGetUniformLocation( m_program, _name.data() ), _value );
	}

	void cShader_opengl::setFloatVector4( const std::string& _name, const cVector4f& _vector ) const
	{
		DF_ProfilingScopeCpu;

		glUniform4f( glGetUniformLocation( m_program, _name.data() ), _vector.x(), _vector.y(), _vector.z(), _vector.w() );
	}

	void cShader_opengl::setFloatVector4( const std::string& _name, const int _size, const float* _value ) const
	{
		DF_ProfilingScopeCpu;

		glUniform4fv( glGetUniformLocation( m_program, _name.data() ), _size, _value );
	}

	void cShader_opengl::setFloatColor( const std::string& _name, const cColor& _color ) const
	{
		DF_ProfilingScopeCpu;

		glUniform4f( glGetUniformLocation( m_program, _name.data() ), _color.r, _color.g, _color.b, _color.a );
	}

	void cShader_opengl::setFloatMatrix4( const std::string& _name, const cMatrix4f& _matrix, const int _amount, const bool _transpose ) const
	{
		DF_ProfilingScopeCpu;

		glUniformMatrix4fv( glGetUniformLocation( m_program, _name.data() ), _amount, _transpose, _matrix.data() );
	}

	void cShader_opengl::setFloatMatrix4( const std::string& _name, const int _size, const float* _value, const bool _transpose ) const
	{
		DF_ProfilingScopeCpu;

		glUniformMatrix4fv( glGetUniformLocation( m_program, _name.data() ), _size, _transpose, _value );
	}

	void cShader_opengl::createProgram( const unsigned _vertex_shader, const unsigned _fragment_shader )
	{
		DF_ProfilingScopeCpu;

		m_program = glCreateProgram();
		glAttachShader( m_program, _vertex_shader );
		glAttachShader( m_program, _fragment_shader );
		glLinkProgram( m_program );

		int success;
		glGetProgramiv( m_program, GL_LINK_STATUS, &success );

		if( success )
			DF_LogMessage( fmt::format( "Successfully linked shader program: {}", m_name ) );
		else
		{
			char info_log[ 512 ];
			glGetProgramInfoLog( m_program, 512, nullptr, info_log );
			DF_LogError( fmt::format( "Failed to link shader program: {} - {}", m_name, info_log ) );
		}

		glDeleteShader( _vertex_shader );
		glDeleteShader( _fragment_shader );
	}

	unsigned cShader_opengl::compileShader( const std::string& _name, const int _type )
	{
		DF_ProfilingScopeCpu;

		std::string       slang_shader_path = cFileSystem::getPath( _name + ".slang" );
		std::ifstream     inputFile( slang_shader_path );
		std::stringstream buffer;
		buffer << inputFile.rdbuf();
		std::string originalContent = buffer.str();
		inputFile.close();
		std::string slang_shader_source = "#define DF_OpenGL\n" + originalContent;

		static Slang::ComPtr< slang::IGlobalSession > slang_global_session;
		if( !slang_global_session.get() ) createGlobalSession( slang_global_session.writeRef() );

		const slang::TargetDesc target_desc{
			.format  = SLANG_SPIRV,
			.profile = slang_global_session->findProfile( "glsl_450" ),
			.flags   = 0,
		};

		slang::CompilerOptionEntry option_entry{
			.name  = slang::CompilerOptionName::Optimization,
			.value = {
				.kind      = slang::CompilerOptionValueKind::Int,
				.intValue0 = SLANG_OPTIMIZATION_LEVEL_NONE,
			},
		};

		const slang::SessionDesc session_desc{
			.targets                  = &target_desc,
			.targetCount              = 1,
			.defaultMatrixLayoutMode  = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR,
			.compilerOptionEntries    = &option_entry,
			.compilerOptionEntryCount = 1,
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
		session->createCompositeComponentType( component_types.data(), static_cast< SlangInt >( component_types.size() ), composed_program.writeRef(), diagnostic_blob.writeRef() );

		Slang::ComPtr< slang::IBlob > spirv_code;
		composed_program->getEntryPointCode( 0, 0, spirv_code.writeRef(), diagnostic_blob.writeRef() );

		const uint32_t* spirv_binary = static_cast< const uint32_t* >( spirv_code->getBufferPointer() );
		const size_t    spirv_size   = spirv_code->getBufferSize() / sizeof( uint32_t );

		spirv_cross::CompilerGLSL          shader( spirv_binary, spirv_size );
		spirv_cross::CompilerGLSL::Options options{};
		options.version                               = 450;
		options.separate_shader_objects               = true;
		options.emit_push_constant_as_uniform_buffer  = true;
		options.emit_uniform_buffer_as_plain_uniforms = false;

		shader.set_common_options( options );
		shader.build_combined_image_samplers();

		spirv_cross::SmallVector< spirv_cross::CombinedImageSampler > image_samplers = shader.get_combined_image_samplers();
		unsigned                                                      binding        = 0;
		for( spirv_cross::CombinedImageSampler& image_sampler: image_samplers )
			shader.set_decoration( image_sampler.combined_id, spv::DecorationBinding, binding++ );

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
