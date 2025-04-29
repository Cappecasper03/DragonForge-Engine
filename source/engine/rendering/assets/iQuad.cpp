#include "iQuad.h"

#include "engine/misc/cTransform.h"
#include "engine/profiling/ProfilingMacros.h"
#include "engine/rendering/assets/iTexture.h"
#include "engine/rendering/vulkan/cRenderer_vulkan.h"

namespace df
{
	iQuad::iQuad( std::string _name, const cVector3f& _position, const cVector2f& _size, const cColor& _color )
		: iRenderAsset( std::move( _name ) )
		, texture( nullptr )
		, color( _color )
		, m_indices{ 0, 1, 3, 1, 2, 3 }
	{
		DF_ProfilingScopeCpu;

		transform->local = transform->world.translated( _position );
		transform->update();

		m_vertices.emplace_back( cVector3f( _size.x() / 2, _size.y() / 2, 0 ), cVector2f( 1, 1 ) );
		m_vertices.emplace_back( cVector3f( _size.x() / 2, -_size.y() / 2, 0 ), cVector2f( 1, 0 ) );
		m_vertices.emplace_back( cVector3f( -_size.x() / 2, -_size.y() / 2, 0 ), cVector2f( 0, 0 ) );
		m_vertices.emplace_back( cVector3f( -_size.x() / 2, _size.y() / 2, 0 ), cVector2f( 0, 1 ) );
	}

	iQuad::~iQuad()
	{
		DF_ProfilingScopeCpu;

		delete texture;
	}
}
