#pragma once

#include "iRenderer.h"
#include "engine/misc/Misc.h"

struct GLFWwindow;

namespace df
{
    class cVulkanRenderer final : public iRenderer
    {
    public:
        DF_DISABLE_COPY_AND_MOVE( cVulkanRenderer )

        cVulkanRenderer();
        ~cVulkanRenderer() override;

        void render() override {}

        void*             getWindow() override { return m_window; }
        const glm::ivec2& getWindowSize() override { return m_window_size; }

    private:
        GLFWwindow* m_window;
    };
}
