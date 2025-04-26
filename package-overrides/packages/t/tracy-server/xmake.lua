package("tracy-server")
    set_homepage("https://github.com/wolfpld/tracy")
    set_description("C++ frame profiler (server code)")

    add_urls("https://github.com/wolfpld/tracy/archive/refs/tags/$(version).tar.gz",
             "https://github.com/wolfpld/tracy.git")

    add_versions("v0.11.1", "2c11ca816f2b756be2730f86b0092920419f3dabc7a7173829ffd897d91888a1")
    add_versions("v0.11.0", "b591ef2820c5575ccbf17e2e7a1dc1f6b9a2708f65bfd00f4ebefad2a1ccf830")

    add_deps("cmake")
    add_deps("freetype", "capstone")

    on_install("windows|x64", "macosx", "linux|x86_64", function (package)
        local configs = {}
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))

        io.writefile("CMakeLists.txt", [[
            cmake_minimum_required(VERSION 3.16)

            option(NO_ISA_EXTENSIONS "Disable ISA extensions (don't pass -march=native or -mcpu=native to the compiler)" OFF)
            option(NO_STATISTICS "Disable calculation of statistics" ON)
            option(NO_PARALLEL_STL "Disable parallel STL" OFF)
            option(NO_FILESELECTOR "Disable the file selector" ON)

            include(cmake/version.cmake)

            set(CMAKE_CXX_STANDARD 20)
            project(TracyServer LANGUAGES C CXX VERSION ${TRACY_VERSION_STRING})

            include(cmake/config.cmake)
            include(cmake/vendor.cmake)
            include(cmake/server.cmake)
        ]])

        os.cp("server", package:installdir("include/tracy"))
        os.cp("public/common", package:installdir("include/tracy/public"))
        os.cp("zstd", package:installdir("include/tracy"))
        import("package.tools.cmake").install(package, configs)
    end)

    on_test(function (package)
        assert(package:check_cxxsnippets({test = [[
            #include <tracy/server/TracyWorker.hpp>
            #include <tracy/server/TracyFileWrite.hpp>

            static void test() {
                tracy::Worker worker( "127.0.0.1", 8086, -1 );
                tracy::FileWrite::Open( "test", tracy::FileCompression::Zstd, 3, 4 );
            }
        ]]}, {configs = {languages = "c++17"}}))
    end)
