#include "cQuad.h"

#include <glad/glad.h>

#include "engine/managers/assets/cQuadManager.h"
#include "engine/managers/cRenderCallbackManager.h"
#include "engine/rendering/OpenGL/cShader.h"
#include "engine/rendering/vulkan/cRenderer.h"

namespace df::opengl
{
	cQuad::cQuad( std::string _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color )
		: iQuad( std::move( _name ), _position, _size, _color )
	{
		ZoneScoped;

		glBindVertexArray( vertex_array );

		glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer );
		glBufferData( GL_ARRAY_BUFFER, sizeof( *m_vertices.data() ) * m_vertices.size(), m_vertices.data(), GL_STATIC_DRAW );

		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, element_buffer );
		glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( m_indices.data() ) * m_indices.size(), m_indices.data(), GL_STATIC_DRAW );

		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( *m_vertices.data() ), nullptr );
		glEnableVertexAttribArray( 0 );

		glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( *m_vertices.data() ), reinterpret_cast< void* >( sizeof( m_vertices[ 0 ].position ) ) );
		glEnableVertexAttribArray( 1 );

		glBindVertexArray( 0 );
	}

	void cQuad::render()
	{
		ZoneScoped;

		if( cQuadManager::getForcedRenderCallback() )
			cRenderCallbackManager::render< cShader >( cQuadManager::getForcedRenderCallback(), this );
		else if( render_callback )
			cRenderCallbackManager::render< cShader >( render_callback, this );
		else
			cRenderCallbackManager::render< cShader >( cQuadManager::getDefaultRenderCallback(), this );
	}
}
