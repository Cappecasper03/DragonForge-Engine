#include "cCamera.h"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>

#include "core/managers/cCameraManager.h"
#include "core/managers/cEventManager.h"

#if PROFILING
#include "core/profiling/Profiling.h"
#endif

namespace df
{
    cCamera::cCamera( std::string _name, const eType& _type, const cColor& _clear_color, const float& _fov, const float& _near_clip, const float& _far_clip )
    : iAsset( std::move( _name ) ),
      view( 1 ),
      projection( 1 ),
      projection_view( 1 ),
      clear_color( _clear_color ),
      type( _type ),
      fov( _fov / 2 ),
      aspect_ratio( 0 ),
      near_clip( _near_clip ),
      far_clip( _far_clip )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        cEventManager::subscribe( event::on_window_resize, this, &cCamera::onWindowResize );
    }

    void cCamera::update()
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        transform.update();

        view = inverse( transform.world );

        projection_view = type == kPerspective ? projection * view : projection;
    }

    void cCamera::beginRender( const int& _clear_buffers )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        cCameraManager* manager = cCameraManager::getInstance();
        m_previus               = manager->current;
        manager->current        = this;

        glClearColor( clear_color.r, clear_color.g, clear_color.b, clear_color.a );
        glClear( _clear_buffers );
    }

    void cCamera::endRender() const
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        cCameraManager::getInstance()->current = m_previus;
    }

    void cCamera::calculateProjection()
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        switch( type )
        {
            case kPerspective: { projection = glm::perspective( glm::radians( fov ), aspect_ratio, near_clip, far_clip ); }
            break;
            case kOrthographic: { projection = glm::ortho( 0.f, ortographic_size.x, 0.f, ortographic_size.y, near_clip, far_clip ); }
        }
    }

    void cCamera::onWindowResize( const int& _width, const int& _height )
    {
#if PROFILING
        PROFILING_SCOPE( __FUNCTION__ );
#endif

        aspect_ratio       = static_cast< float >( _width ) / static_cast< float >( _height );
        ortographic_size.x = static_cast< float >( _width );
        ortographic_size.y = static_cast< float >( _height );
        calculateProjection();
    }
}
