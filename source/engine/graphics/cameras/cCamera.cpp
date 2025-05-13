#include "cCamera.h"

#include "engine/core/math/math.h"
#include "engine/core/utils/cTransform.h"
#include "engine/graphics/api/iRenderer.h"
#include "engine/graphics/cRenderer.h"
#include "engine/managers/cCameraManager.h"
#include "engine/managers/cEventManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	cCamera::cCamera( std::string _name, const eType _type, const cColor& _clear_color, const float _fov, const float _near_clip, const float _far_clip )
		: iAsset( std::move( _name ) )
		, m_view( 1 )
		, m_projection( 1 )
		, m_view_projection( 1 )
		, m_clear_color( _clear_color )
		, m_type( _type )
		, m_transform( new cTransform() )
		, m_fov( _fov / 2 )
		, m_aspect_ratio( 0 )
		, m_near_clip( _near_clip )
		, m_far_clip( _far_clip )
	{
		DF_ProfilingScopeCpu;

		cEventManager::subscribe( event::on_window_resize, this, &cCamera::onWindowResize );
	}

	cCamera::~cCamera()
	{
		DF_ProfilingScopeCpu;

		delete m_transform;
	}

	void cCamera::update( const float /*_delta_time*/ )
	{
		DF_ProfilingScopeCpu;

		m_transform->update();

		m_view = m_transform->m_world.inversed();

		m_view_projection = m_type == kPerspective ? m_projection * m_view : m_projection;
	}

	void cCamera::beginRender( const int _clear_buffers )
	{
		DF_ProfilingScopeCpu;

		cCameraManager* manager = cCameraManager::getInstance();
		m_previous               = manager->m_current;
		manager->m_current        = this;

		cRenderer::getRenderInstance()->beginRendering( _clear_buffers, m_clear_color );
	}

	void cCamera::endRender()
	{
		DF_ProfilingScopeCpu;

		cRenderer::getRenderInstance()->endRendering();

		cCameraManager::getInstance()->m_current = m_previous;
		m_previous                              = nullptr;
	}

	void cCamera::calculateProjection()
	{
		DF_ProfilingScopeCpu;

		switch( m_type )
		{
			case kPerspective:
			{
				m_projection = cMatrix4f::createPerspective( math::radians( m_fov ), m_aspect_ratio, m_near_clip, m_far_clip );

				if( cRenderer::getInstanceType() & cRenderer::eInstanceType::kVulkan )
					m_projection.up().y() *= -1;
			}
			break;
			case kOrthographic:
			{
				m_projection = cMatrix4f::createOrtho( 0.f, m_orthographic_size.x(), 0.f, m_orthographic_size.y(), m_near_clip, m_far_clip );

				if( cRenderer::getInstanceType() & cRenderer::eInstanceType::kVulkan )
				{
					const cMatrix4f correction( cVector4f( 1.0f, 0.0f, 0.0f, 0.0f ),
					                            cVector4f( 0.0f, -1.0f, 0.0f, 0.0f ),
					                            cVector4f( 0.0f, 0.0f, 0.5f, 0.0f ),
					                            cVector4f( 0.0f, 0.0f, 0.5f, 1.0f ) );

					m_projection = correction * m_projection;
				}
			}
		}
	}

	void cCamera::onWindowResize( const int _width, const int _height )
	{
		DF_ProfilingScopeCpu;

		m_aspect_ratio         = static_cast< float >( _width ) / static_cast< float >( _height );
		m_orthographic_size.x() = static_cast< float >( _width );
		m_orthographic_size.y() = static_cast< float >( _height );
		calculateProjection();
		update();
	}
}
