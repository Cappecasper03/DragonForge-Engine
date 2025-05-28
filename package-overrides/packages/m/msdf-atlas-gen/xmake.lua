package("msdf-atlas-gen")
    set_homepage("https://github.com/Chlumsky/msdf-atlas-gen")
    set_description("Multi-channel signed distance field atlas generator")
    set_license("MIT")

    add_urls("https://github.com/Chlumsky/msdf-atlas-gen/archive/refs/tags/$(version).tar.gz",
             "https://github.com/Chlumsky/msdf-atlas-gen.git")

    add_versions("v1.3", "5d3d58e8bc92836baf23ce3a80ef79cc4c2d022fb86b7f160b11cc06cd62fe78")

    add_deps("cmake")
    add_deps("zlib")
    add_deps("msdfgen", {configs = {extensions = true}})

    on_install(function (package)
        local configs = {}
        table.insert(configs, "-DMSDF_ATLAS_USE_VCPKG=OFF")
        table.insert(configs, "-DMSDF_ATLAS_INSTALL=ON")
        table.insert(configs, "-DMSDF_ATLAS_DYNAMIC_RUNTIME=OFF")
        table.insert(configs, "-DMSDF_ATLAS_MSDFGEN_EXTERNAL=ON")
        table.insert(configs, "-DMSDF_ATLAS_NO_ARTERY_FONT=ON")
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))

        if package:is_plat("windows") then
            io.replace("CMakeLists.txt", [[set(MSDFGEN_MSVC_RUNTIME "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")]], "", {plain = true})
            io.replace("CMakeLists.txt", [[set(MSDFGEN_MSVC_RUNTIME "MultiThreaded$<$<CONFIG:Debug>:Debug>")]], "", {plain = true})
        elseif package:is_plat("mingw") then
            io.replace("CMakeLists.txt", [[target_compile_definitions(msdfgen-core PRIVATE "MSDFGEN_PUBLIC=__declspec(dllexport)")]], "target_compile_definitions(msdfgen-core PRIVATE MSDFGEN_PUBLIC=)", {plain = true})
        end

        import("package.tools.cmake").install(package, configs)
    end)

    on_test(function (package)
        assert(package:check_cxxsnippets({test = [[
            #include <msdf-atlas-gen/msdf-atlas-gen.h>

            static void test() {
                msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
                msdfgen::deinitializeFreetype(ft);
            }
        ]]}, {configs = {languages = "c++17"}}))
    end)
