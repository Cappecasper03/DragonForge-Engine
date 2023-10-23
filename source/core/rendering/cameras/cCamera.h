#pragma once

#include "core/misc/cColor.h"
#include "core/misc/cTransform.h"

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

        explicit cCamera( const eType& _type, const cColor& _clear_color, const float& _fov, const float& _near_clip = .1f, const float& _far_clip = 100 );
        virtual  ~cCamera() = default;

        virtual void update();

        void beginRender( const int& _clear_buffers );
        void endRender() const;

        cTransform transform;

        glm::mat4 view;
        glm::mat4 projection;
        glm::mat4 projection_view;

        cColor clear_color;

        eType     type;
        float     fov;
        float     aspect_ratio;
        float     near_clip;
        float     far_clip;
        glm::vec2 ortographic_size;

    protected:
        void calculateProjection();

    private:
        void onWindowResize( const int& _width, const int& _height );

        cCamera* m_previus;
    };
}
