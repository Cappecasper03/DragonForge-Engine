filter {}
    includedirs { "../../assimp/include" }
    libdirs { "../../assimp/lib" }

filter "configurations:Debug"
    links { "assimp-vc143-mt-d.lib" }

filter "configurations:Release"
    links { "assimp-vc143-mt.lib" }