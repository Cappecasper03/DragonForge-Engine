#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "core/misc/iSingleton.h"

namespace df
{
    namespace event
    {
        static std::string input     = "input";
        static std::string update    = "update";
        static std::string render_3d = "render3D";
        static std::string render_2d = "render2D";
    }

    class cEventManager final : public iSingleton< cEventManager >
    {
    public:
        DISABLE_COPY_AND_MOVE( cEventManager );

        cEventManager() = default;
        ~cEventManager() override;

        static void subscribe( const std::string& _name, void* _object, std::function< void( void* ) >& _function );
        static void unsubscribe( const std::string& _name, void* _object );

        static void invoke( const std::string& _name, void* _data );

    private:
        struct sEvent
        {
            std::string                                                       name;
            std::unordered_map< const void*, std::function< void( void* ) > > subscribers;
        };

        std::unordered_map< std::string, sEvent* > m_events;
    };
}
