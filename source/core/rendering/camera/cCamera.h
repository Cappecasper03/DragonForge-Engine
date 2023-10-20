#pragma once

#include "core/misc/cTransform.h"

struct GLFWwindow;

namespace df
{
    class cCamera
    {
    public:
        DISABLE_COPY_AND_MOVE( cCamera );

        enum eType
        {
            kPerspective,
            kOrthographic
        };

        explicit cCamera( const eType& _type, const float& _fov, const float& _near_clip = .1f, const float& _far_clip = 100 );
        virtual  ~cCamera() = default;

        virtual void update();

        virtual bool beginRender();
        virtual void endRender();

        cTransform transform;

        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 view_projection;

        eType     type;
        float     fov;
        float     aspect_ratio;
        float     near_clip;
        float     far_clip;
        glm::vec2 ortographic_size;

    private:
        void onWindowResize( GLFWwindow* _window, const int _width, const int _height );

        void calculateProjection();
    };
}
