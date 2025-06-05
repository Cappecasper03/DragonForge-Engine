#include "cSampler_opengl.h"

#include <glad/glad.h>

#include "engine/profiling/ProfilingMacros.h"

namespace df::opengl
{
	cSampler_opengl::cSampler_opengl()
	{
		DF_ProfilingScopeCpu;

		glGenSamplers( 1, &m_id );
	}

	cSampler_opengl::~cSampler_opengl()
	{
		DF_ProfilingScopeCpu;

		glDeleteSamplers( 1, &m_id );
	}

	void cSampler_opengl::update()
	{
		DF_ProfilingScopeCpu;

		for( const std::pair< const sSamplerParameter::eName, sSamplerParameter::eParameter >& parameter: m_parameters )
			glSamplerParameteri( m_id, sSamplerParameter::toOpenGl( parameter.first ), sSamplerParameter::toOpenGl( parameter.second ) );
	}

	void cSampler_opengl::bind( const unsigned _index )
	{
		DF_ProfilingScopeCpu;

		glActiveTexture( GL_TEXTURE0 + _index );
		glBindSampler( _index, m_id );
	}
}