#pragma once

namespace df
{
    class cCamera
    {
    public:
        cCamera();
        virtual ~cCamera() = default;

    protected:
        float m_fov;
    };
}
