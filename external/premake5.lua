project "GLFW"
    location "glfw"
    kind "StaticLib"
    language "C"
    warnings "Default"

	targetname "glfw3"
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
	{
		"glfw/include/GLFW/glfw3.h",
		"glfw/include/GLFW/glfw3native.h",     
        "glfw/src/context.c",
        "glfw/src/init.c",
        "glfw/src/input.c",
        "glfw/src/monitor.c",
        "glfw/src/null_init.c",
        "glfw/src/null_joystick.c",
        "glfw/src/null_monitor.c",
        "glfw/src/null_window.c",
        "glfw/src/platform.c",
        "glfw/src/vulkan.c",
        "glfw/src/window.c"
	}
    
	filter "system:windows"
		staticruntime "On"

		files
		{
			"glfw/src/win32_init.c",
			"glfw/src/win32_joystick.c",
            "glfw/src/win32_module.c",
			"glfw/src/win32_monitor.c",
			"glfw/src/win32_thread.c",
			"glfw/src/win32_time.c",
			"glfw/src/win32_window.c",
			"glfw/src/wgl_context.c",
			"glfw/src/egl_context.c",
			"glfw/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

    filter "system:linux"
        pic "On"

        systemversion "latest"
        staticruntime "On"

        files
        {
            "glfw/src/x11_init.c",
            "glfw/src/x11_monitor.c",
            "glfw/src/x11_window.c",
            "glfw/src/xkb_unicode.c",
            "glfw/src/posix_time.c",
            "glfw/src/posix_module.c",
            "glfw/src/posix_thread.c",
            "glfw/src/posix_poll.c",
            "glfw/src/glx_context.c",
            "glfw/src/egl_context.c",
            "glfw/src/osmesa_context.c",
            "glfw/src/linux_joystick.c"
        }

        defines
        {
            "_GLFW_X11"
        }

	filter "configurations:Debug_GL or Debug_DX"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release_GL or Release_DX"
		runtime "Release"
		optimize "on"

project "glad"
    location "glad"
    kind "StaticLib"
    language "C"
    warnings "Default"

    targetname "glad"
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "glad/**.h",
        "glad/**.c"
    }

    includedirs "glad/include"

    staticruntime "On"

    filter "system:linux"
        pic "On"
        systemversion "latest"

    filter "configurations:Debug_GL or Debug_DX"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release_GL or Release_DX"
        runtime "Release"
        optimize "on"

project "imgui"
    location "imgui"
    kind "StaticLib"
    language "C++"
    warnings "Default"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "imgui/*.h",
        "imgui/*.cpp"
    }

    includedirs
    {
        "imgui",
        "glfw/include",
    }

    files
    {
        "imgui/backends/imgui_impl_glfw.h",
        "imgui/backends/imgui_impl_glfw.cpp",
        "imgui/backends/imgui_impl_opengl3.h",
        "imgui/backends/imgui_impl_opengl3.cpp",
        "imgui/backends/imgui_impl_dx11.h",
        "imgui/backends/imgui_impl_dx11.cpp"
    }
    
    filter "system:windows"
        staticruntime "On"
    
    filter "system:linux"
        pic "On"
        staticruntime "On"

    filter "configurations:Debug_GL or Debug_DX"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release_GL or Release_DX"
        runtime "Release"
        optimize "on"

project "base64"
    location "base64"
    kind "StaticLib"
    language "C++"
    warnings "Default"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "base64/**.h",
        "base64/**.cpp"
    }

    includedirs "base64/include"
    
    filter "system:windows"
        staticruntime "On"
    
    filter "system:linux"
        pic "On"
        staticruntime "On"

    filter "configurations:Debug_GL or Debug_DX"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release_GL or Release_DX"
        runtime "Release"
        optimize "on"