filter {}
    externalincludedirs  { "../include" }
    libdirs { "../lib" }
    files { "../src/glad.c" }
    links { "opengl32.lib" }

filter "configurations:Debug"
    links { "assimp-vc143-mt-d.lib", "glfw3-d.lib" }

filter "configurations:Release"
    links { "assimp-vc143-mt.lib", "glfw3.lib" }