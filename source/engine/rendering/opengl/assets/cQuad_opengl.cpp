﻿#include "cQuad_opengl.h"

#include <glad/glad.h>

#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/rendering/OpenGL/cShader_opengl.h"
#include "engine/rendering/vulkan/cRenderer_vulkan.h"

namespace df::opengl
{
	cQuad_opengl::cQuad_opengl( std::string _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color )
		: iQuad( std::move( _name ), _position, _size, _color )
	{
		ZoneScoped;

		glBindVertexArray( vertex_array );

		glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof( sVertex ) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW );

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, element_buffer );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned ) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW );

		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( sVertex ), nullptr );
		glEnableVertexAttribArray( 0 );

		glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( sVertex ), reinterpret_cast< void* >( offsetof( sVertex, sVertex::tex_coord ) ) );
		glEnableVertexAttribArray( 1 );

		glBindVertexArray( 0 );
	}

	void cQuad_opengl::render()
	{
		ZoneScoped;

		if( cQuadManager::getForcedRenderCallback() )
			cRenderCallbackManager::render< cShader_opengl >( cQuadManager::getForcedRenderCallback(), this );
		else if( render_callback )
			cRenderCallbackManager::render< cShader_opengl >( render_callback, this );
		else
			cRenderCallbackManager::render< cShader_opengl >( cQuadManager::getDefaultRenderCallback(), this );
	}
}
