#include "iQuad.h"

#include "engine/core/utils/cTransform.h"
#include "engine/profiling/ProfilingMacros.h"
#include "textures/iTexture.h"

namespace df
{
	iQuad::iQuad( const std::string& _name, const cVector3f& _position, const cVector2f& _size, const cColor& _color )
		: iAsset( _name )
		, m_texture( nullptr )
		, m_color( _color )
		, m_indices{ 0, 1, 3, 1, 2, 3 }
	{
		DF_ProfilingScopeCpu;

		m_transform.m_local = m_transform.m_world.translated( _position );
		m_transform.update();

		m_vertices.emplace_back( cVector3f( _size.x() / 2, _size.y() / 2, 0 ), cVector2f( 1, 1 ) );
		m_vertices.emplace_back( cVector3f( _size.x() / 2, -_size.y() / 2, 0 ), cVector2f( 1, 0 ) );
		m_vertices.emplace_back( cVector3f( -_size.x() / 2, -_size.y() / 2, 0 ), cVector2f( 0, 0 ) );
		m_vertices.emplace_back( cVector3f( -_size.x() / 2, _size.y() / 2, 0 ), cVector2f( 0, 1 ) );
	}

	iQuad::~iQuad()
	{
		DF_ProfilingScopeCpu;

		delete m_texture;
	}
}
