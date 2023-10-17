#pragma once

#include <vector>

#include "cMatrix4x4.h"

namespace df
{
    class cTransform final
    {
    public:
        cTransform();
        cTransform( const cTransform& _other ) = delete;
        cTransform( cTransform&& _other )      = default;
        ~cTransform();

        cTransform& operator=( const cTransform& )   = delete;
        cTransform& operator=( cTransform&& _other ) = default;

        void update();

        bool addChild( cTransform& _child );
        bool removeChild( cTransform& _child );

        bool setParent( cTransform& _parent );
        bool removeParent();

        const cMatrix4X4f& getWorldMatrix() { return m_render_matrix; }

        cMatrix4X4f matrix;

    private:
        cMatrix4X4f                m_render_matrix;
        cTransform*                m_parent;
        std::vector< cTransform* > m_children;
    };
}
