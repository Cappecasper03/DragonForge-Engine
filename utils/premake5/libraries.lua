filter {}
    externalincludedirs  { "../include" }
    libdirs { "../lib" }
    files { "../src/glad.c" }
    links { "opengl32.lib" }

filter "configurations:Debug"
    links { "assimp-vc143-mtd.lib",
            "glfw3d.lib",
            "freetyped.lib" }

filter "configurations:Release"
    links { "assimp-vc143-mt.lib",
            "glfw3.lib",
            "freetype.lib" }