#include "cCamera.h"

#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>

#include "engine/managers/cEventManager.h"
#include "engine/managers/assets/cCameraManager.h"

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
        cEventManager::subscribe( event::on_window_resize, this, &cCamera::onWindowResize );
    }

    void cCamera::update( const float& /*_delta_time*/ )
    {
        transform.update();

        view = inverse( transform.world );

        projection_view = type == kPerspective ? projection * view : projection;
    }

    void cCamera::beginRender( const int& _clear_buffers )
    {
        cCameraManager* manager = cCameraManager::getInstance();
        m_previus               = manager->current;
        manager->current        = this;

        glClearColor( clear_color.r, clear_color.g, clear_color.b, clear_color.a );
        glClear( _clear_buffers );
    }

    void cCamera::endRender()
    {
        cCameraManager::getInstance()->current = m_previus;
        m_previus                              = nullptr;
    }

    void cCamera::calculateProjection()
    {
        switch( type )
        {
            case kPerspective: { projection = glm::perspective( glm::radians( fov ), aspect_ratio, near_clip, far_clip ); }
            break;
            case kOrthographic: { projection = glm::ortho( 0.f, ortographic_size.x, 0.f, ortographic_size.y, near_clip, far_clip ); }
        }
    }

    void cCamera::onWindowResize( const int& _width, const int& _height )
    {
        aspect_ratio       = static_cast< float >( _width ) / static_cast< float >( _height );
        ortographic_size.x = static_cast< float >( _width );
        ortographic_size.y = static_cast< float >( _height );
        calculateProjection();
        update();
    }
}
