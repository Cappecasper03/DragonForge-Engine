#include "cCameraManager.h"

#include <format>
#include <ranges>

#include "core/rendering/camera/cCamera.h"

namespace df
{
    cCameraManager::cCameraManager()
    : current( nullptr )
    {
        current = create( "default3D", cCamera::eType::kPerspective, cColor( .5f, .75f, 1, 1 ), 90 );
        create( "default2D", cCamera::eType::kOrthographic, cColor( .5f, .75f, 1, 1 ), 90 );
    }

    cCameraManager::~cCameraManager()
    {
        for( const std::string& name : m_cameras | std::ranges::views::keys )
            destroy( name );
    }

    cCamera* cCameraManager::create( const std::string& _name, const cCamera::eType& _type, const cColor& _clear_color, const float& _fov, const float& _near_clip, const float& _far_clip )
    {
        if( m_cameras.contains( _name ) )
        {
            LOG_WARNING( std::format( "Camera already exist: {}", _name ) );
            return nullptr;
        }

        cCamera* camera    = MEMORY_ALLOC( cCamera, 1, _type, _clear_color, _fov, _near_clip, _far_clip );
        m_cameras[ _name ] = camera;

        LOG_MESSAGE( std::format( "Created camera: {}", _name ) );
        return camera;
    }

    bool cCameraManager::add( const std::string& _name, cCamera* _camera )
    {
        if( m_cameras.contains( _name ) )
        {
            LOG_WARNING( std::format( "Camera already exist: {}", _name ) );
            return false;
        }

        LOG_MESSAGE( std::format( "Added camera: {}", _name ) );
        m_cameras[ _name ] = _camera;
        return true;
    }

    bool cCameraManager::destroy( const std::string& _name )
    {
        if( m_cameras.erase( _name ) )
        {
            LOG_MESSAGE( std::format( "Destroyed camera: {}", _name ) );
            return true;
        }

        LOG_WARNING( std::format( "Camera doesn't exist: {}", _name ) );
        return false;
    }

    bool cCameraManager::destroy( const cCamera* _camera )
    {
        for( const std::pair< const std::string, cCamera* >& camera : m_cameras )
        {
            if( camera.second == _camera )
            {
                m_cameras.erase( camera.first );
                return true;
            }
        }

        return false;
    }

    cCamera* cCameraManager::get( const std::string& _name )
    {
        const auto it = m_cameras.find( _name );
        if( it == m_cameras.end() )
        {
            LOG_WARNING( std::format( "Camera doesn't exist: {}", _name ) );
            return nullptr;
        }

        return it->second;
    }
}
