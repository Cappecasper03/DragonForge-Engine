#pragma once

#include <vector>
#include <glm/mat4x4.hpp>

#include "core/misc/Misc.h"

namespace df
{
    class cTransform final
    {
    public:
        DISABLE_COPY( cTransform )

        cTransform();
        cTransform( cTransform&& _other ) = default;
        ~cTransform();

        cTransform& operator=( cTransform&& _other ) = default;

        void update();

        bool addChild( cTransform& _child );
        bool removeChild( cTransform& _child );

        bool setParent( cTransform& _parent );
        bool removeParent();

        glm::mat4x4 local;
        glm::mat4x4 world;

        cTransform*                parent;
        std::vector< cTransform* > children;
    };
}
