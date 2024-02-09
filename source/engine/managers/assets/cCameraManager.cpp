#include "cCameraManager.h"

#include "engine/rendering/assets/cameras/cCamera.h"

namespace df
{
    cCameraManager::cCameraManager()
    {
        ZoneScoped;

        current = create( "default_3d", cCamera::eType::kPerspective, cColor( .5f, .75f, 1, 1 ), 90.f );
        create( "default_2d", cCamera::eType::kOrthographic, color::white, 90.f, 0.f, 100.f );
    }

    bool cCameraManager::add( cCamera* _camera )
    {
        ZoneScoped;

        std::unordered_map< std::string, iAsset* >& assets = getInstance()->m_assets;

        if( assets.contains( _camera->name ) )
        {
            DF_LOG_WARNING( std::format( "Camera already exist: {}", _camera->name ) );
            return false;
        }

        assets[ _camera->name ] = _camera;

        DF_LOG_MESSAGE( std::format( "Created asset: {}", _camera->name ) );
        return true;
    }
}
