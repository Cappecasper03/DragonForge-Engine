#include "cCameraManager.h"

#include <format>

#include "core/rendering/cameras/cCamera.h"

namespace df
{
    cCameraManager::cCameraManager()
    : current( nullptr )
    {
        current = create( "default3D", cCamera::eType::kPerspective, cColor( .5f, .75f, 1, 1 ), 90 );
        create( "default2D", cCamera::eType::kOrthographic, cColor( .5f, .75f, 1, 1 ), 90, 0, 100 );
    }

    cCamera* cCameraManager::create( const std::string& _name, const cCamera::eType& _type, const cColor& _clear_color, const float& _fov, const float& _near_clip, const float& _far_clip )
    {
        std::unordered_map< std::string, cCamera* >& cameras = getInstance()->m_cameras;

        if( cameras.contains( _name ) )
        {
            LOG_WARNING( std::format( "Camera already exist: {}", _name ) );
            return nullptr;
        }

        cCamera* camera  = MEMORY_ALLOC( cCamera, 1, _type, _clear_color, _fov, _near_clip, _far_clip );
        cameras[ _name ] = camera;

        LOG_MESSAGE( std::format( "Created camera: {}", _name ) );
        return camera;
    }

    bool cCameraManager::destroy( const std::string& _name )
    {
        std::unordered_map< std::string, cCamera* >& cameras = getInstance()->m_cameras;

        const auto it = cameras.find( _name );
        if( it == cameras.end() )
        {
            LOG_WARNING( std::format( "Camera doesn't exist: {}", _name ) );
            return false;
        }

        MEMORY_FREE( it->second );
        cameras.erase( it );
        LOG_MESSAGE( std::format( "Destroyed camera: {}", _name ) );

        return true;
    }

    bool cCameraManager::destroy( const cCamera* _camera )
    {
        std::unordered_map< std::string, cCamera* >& cameras = getInstance()->m_cameras;

        for( const std::pair< const std::string, cCamera* >& camera : cameras )
        {
            if( camera.second == _camera )
            {
                LOG_MESSAGE( std::format( "Destroyed camera: {}", camera.first ) );
                MEMORY_FREE( camera.second );
                cameras.erase( camera.first );
                return true;
            }
        }

        return false;
    }

    void cCameraManager::clear()
    {
        std::unordered_map< std::string, cCamera* >& cameras = getInstance()->m_cameras;

        for( std::pair< const std::string, cCamera* >& camera : cameras )
        {
            LOG_MESSAGE( std::format( "Destroyed camera: {}", camera.first ) );
            MEMORY_FREE( camera.second );
        }

        cameras.clear();
    }

    cCamera* cCameraManager::get( const std::string& _name )
    {
        std::unordered_map< std::string, cCamera* >& cameras = getInstance()->m_cameras;

        const auto it = cameras.find( _name );
        if( it == cameras.end() )
        {
            LOG_WARNING( std::format( "Camera doesn't exist: {}", _name ) );
            return nullptr;
        }

        return it->second;
    }
}
