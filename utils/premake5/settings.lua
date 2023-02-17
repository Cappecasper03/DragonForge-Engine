workspace_name = "workspace" 
project_name = "project" 
cppdialect "C++20" 
 
filter "configurations:Debug" 
    defines { "DEBUG" } 
    symbols "On" 
 
filter "configurations:Release" 
    defines { "NDEBUG" } 
    optimize "On" 
