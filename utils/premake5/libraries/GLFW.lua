filter {}
    includedirs { "../../glfw/include" }
    libdirs { "../../glfw/lib" }

filter "configurations:Debug"
    links { "glfw3-d.lib" }

filter "configurations:Release"
    links { "glfw3.lib" }