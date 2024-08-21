filter {}
	externalincludedirs
	{
		'backends',
		'../GLFW/include',
		'../vulkan/include',
	}
	
	files
	{ 
		'misc/cpp/imgui_stdlib.*',
		'backends/imgui_impl_glfw.*',
		'backends/imgui_impl_opengl3.*',
		'backends/imgui_impl_vulkan.*',
	}