#include "iQuad.h"

#include <glm/ext/matrix_transform.hpp>

#include "engine/managers/cRenderCallbackManager.h"
#include "engine/rendering/assets/iTexture.h"
#include "engine/rendering/Vulkan/cPipeline.h"
#include "engine/rendering/Vulkan/cRenderer.h"

namespace df
{
	iQuad::iQuad( std::string _name, const glm::vec3& _position, const glm::vec2& _size, const cColor& _color )
		: iRenderAsset( std::move( _name ) )
		, texture( nullptr )
		, color( _color )
		, m_vertices{}
		, m_indices{ 0, 1, 3, 1, 2, 3 }
	{
		ZoneScoped;

		transform->local = translate( transform->world, _position );
		transform->update();

		m_vertices[ 0 ] = { glm::vec3( _size.x / 2, _size.y / 2, 0 ), glm::vec2( 1, 1 ) };
		m_vertices[ 1 ] = { glm::vec3( _size.x / 2, -_size.y / 2, 0 ), glm::vec2( 1, 0 ) };
		m_vertices[ 2 ] = { glm::vec3( -_size.x / 2, -_size.y / 2, 0 ), glm::vec2( 0, 0 ) };
		m_vertices[ 3 ] = { glm::vec3( -_size.x / 2, _size.y / 2, 0 ), glm::vec2( 0, 1 ) };
	}
}
