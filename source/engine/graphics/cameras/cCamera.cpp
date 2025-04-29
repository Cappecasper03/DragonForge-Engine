#include "cCamera.h"

#include "core/math/math.h"
#include "engine/managers/assets/cCameraManager.h"
#include "engine/managers/cEventManager.h"
#include "engine/misc/cTransform.h"
#include "engine/profiling/ProfilingMacros.h"
#include "graphics/api/iRenderer.h"
#include "graphics/cRenderer.h"

namespace df
{
	cCamera::cCamera( std::string _name, const eType _type, const cColor& _clear_color, const float _fov, const float _near_clip, const float _far_clip )
		: iAsset( std::move( _name ) )
		, view( 1 )
		, projection( 1 )
		, view_projection( 1 )
		, clear_color( _clear_color )
		, type( _type )
		, transform( new cTransform() )
		, fov( _fov / 2 )
		, aspect_ratio( 0 )
		, near_clip( _near_clip )
		, far_clip( _far_clip )
	{
		DF_ProfilingScopeCpu;

		cEventManager::subscribe( event::on_window_resize, this, &cCamera::onWindowResize );
	}

	cCamera::~cCamera()
	{
		DF_ProfilingScopeCpu;

		delete transform;
	}

	void cCamera::update( const float /*_delta_time*/ )
	{
		DF_ProfilingScopeCpu;

		transform->update();

		view = transform->world.inversed();

		view_projection = type == ePerspective ? projection * view : projection;
	}

	void cCamera::beginRender( const int _clear_buffers )
	{
		DF_ProfilingScopeCpu;

		cCameraManager* manager = cCameraManager::getInstance();
		m_previus               = manager->current;
		manager->current        = this;

		cRenderer::getRenderInstance()->beginRendering( _clear_buffers, clear_color );
	}

	void cCamera::endRender()
	{
		DF_ProfilingScopeCpu;

		cRenderer::getRenderInstance()->endRendering();

		cCameraManager::getInstance()->current = m_previus;
		m_previus                              = nullptr;
	}

	void cCamera::calculateProjection()
	{
		DF_ProfilingScopeCpu;

		switch( type )
		{
			case ePerspective:
			{
				projection = cMatrix4f::createPerspective( math::radians( fov ), aspect_ratio, near_clip, far_clip );

				if( cRenderer::getInstanceType() & cRenderer::eInstanceType::kVulkan )
					projection.up().y() *= -1;
			}
			break;
			case eOrthographic:
			{
				projection = cMatrix4f::createOrtho( 0.f, ortographic_size.x(), 0.f, ortographic_size.y(), near_clip, far_clip );

				if( cRenderer::getInstanceType() & cRenderer::eInstanceType::kVulkan )
				{
					const cMatrix4f correction( cVector4f( 1.0f, 0.0f, 0.0f, 0.0f ),
					                            cVector4f( 0.0f, -1.0f, 0.0f, 0.0f ),
					                            cVector4f( 0.0f, 0.0f, 0.5f, 0.0f ),
					                            cVector4f( 0.0f, 0.0f, 0.5f, 1.0f ) );

					projection = correction * projection;
				}
			}
		}
	}

	void cCamera::onWindowResize( const int _width, const int _height )
	{
		DF_ProfilingScopeCpu;

		aspect_ratio         = static_cast< float >( _width ) / static_cast< float >( _height );
		ortographic_size.x() = static_cast< float >( _width );
		ortographic_size.y() = static_cast< float >( _height );
		calculateProjection();
		update();
	}
}
