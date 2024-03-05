#include "AssetTypes.h"

#include <tracy/Tracy.hpp>

namespace df
{
    iAsset::iAsset( std::string _name )
    : name( std::move( _name ) )
    {}

    iRenderAsset::iRenderAsset( std::string _name )
    : iAsset( std::move( _name ) ),
      transform( new cTransform() ),
      render_callback( nullptr )
    {}

    /*namespace vulkan
    {
        sRendererSpecific::sRendererSpecific()
        : vertex_buffer( nullptr ),
          vertex_buffer_memory( nullptr )
        {
            ZoneScoped;
        }

        sRendererSpecific::~sRendererSpecific()
        {
            ZoneScoped;

            const cRenderer* renderer = reinterpret_cast< cRenderer* >( cRendererSingleton::getRenderInstance() );

            vkDestroyBuffer( renderer->logical_device, vertex_buffer, nullptr );
            vkFreeMemory( renderer->logical_device, vertex_buffer_memory, nullptr );
        }
    }*/

    iRenderAsset::~iRenderAsset()
    {
        ZoneScoped;

        delete transform;
    }
}
