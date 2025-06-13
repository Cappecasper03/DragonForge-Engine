#include "cCamera.h"

#include "engine/core/math/math.h"
#include "engine/core/utils/cTransform.h"
#include "engine/graphics/api/iGraphicsDevice.h"
#include "engine/graphics/cRenderer.h"
#include "engine/managers/cCameraManager.h"
#include "engine/managers/cEventManager.h"
#include "engine/profiling/ProfilingMacros.h"

namespace df
{
	cCamera::cCamera( const sDescription& _description )
		: m_view( 1 )
		, m_projection( 1 )
		, m_view_projection( 1 )
		, m_aspect_ratio( 0 )
		, m_flip_y( false )
		, m_description( _description )
	{
		DF_ProfilingScopeCpu;

		m_description.fov /= 2;

		cEventManager::subscribe( event::on_window_resize, this, &cCamera::onWindowResize );
	}

	void cCamera::update( const float /*_delta_time*/ )
	{
		DF_ProfilingScopeCpu;

		m_transform.update();

		m_view = m_transform.m_world.inversed();

		m_view_projection = m_description.type == kPerspective ? m_projection * m_view : m_projection;
	}

	void cCamera::beginRender( const eClearFlags _clear_flags )
	{
		DF_ProfilingScopeCpu;

		cCameraManager* manager       = cCameraManager::getInstance();
		m_previous                    = manager->m_current;
		manager->m_current            = this;
		manager->m_current_is_regular = true;

		cRenderer::getGraphicsDevice()->beginRendering( _clear_flags, m_description.clear_color );
	}

	void cCamera::endRender()
	{
		DF_ProfilingScopeCpu;

		cRenderer::getGraphicsDevice()->endRendering();

		cCameraManager::getInstance()->m_current = m_previous;
		m_previous                               = nullptr;
	}

	void cCamera::calculateProjection()
	{
		DF_ProfilingScopeCpu;

		switch( m_description.type )
		{
			case kPerspective:
			{
				m_projection = cMatrix4f::createPerspectiveProjection( math::radians( m_description.fov ), m_aspect_ratio, m_description.near_clip, m_description.far_clip );

				if( cRenderer::getDeviceType() != cRenderer::eDeviceType::kVulkan && m_flip_y )
					m_projection.up().y() *= -1;

				if( cRenderer::getDeviceType() == cRenderer::eDeviceType::kVulkan )
					m_projection.up().y() *= -1;
			}
			break;
			case kOrthographic:
			{
				if( m_flip_y )
					m_projection = cMatrix4f::createOrthographicProjection( 0.f,
					                                                        m_orthographic_size.x(),
					                                                        m_orthographic_size.y(),
					                                                        0.f,
					                                                        m_description.near_clip,
					                                                        m_description.far_clip );
				else
					m_projection = cMatrix4f::createOrthographicProjection( 0.f,
					                                                        m_orthographic_size.x(),
					                                                        0.f,
					                                                        m_orthographic_size.y(),
					                                                        m_description.near_clip,
					                                                        m_description.far_clip );

				if( cRenderer::getDeviceType() & cRenderer::eDeviceType::kVulkan )
				{
					const cMatrix4f correction( cVector4f( 1.0f, 0.0f, 0.0f, 0.0f ),
					                            cVector4f( 0.0f, -1.0f, 0.0f, 0.0f ),
					                            cVector4f( 0.0f, 0.0f, 0.5f, 0.0f ),
					                            cVector4f( 0.0f, 0.0f, 0.5f, 1.0f ) );

					m_projection = correction * m_projection;
				}
			}
			case kNone: break;
		}
	}

	void cCamera::onWindowResize( const int _width, const int _height )
	{
		DF_ProfilingScopeCpu;

		m_aspect_ratio          = static_cast< float >( _width ) / static_cast< float >( _height );
		m_orthographic_size.x() = static_cast< float >( _width );
		m_orthographic_size.y() = static_cast< float >( _height );
		calculateProjection();
		update();
	}
}
