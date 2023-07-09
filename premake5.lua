outputdir = "%{cfg.system}-%{cfg.architecture}/%{cfg.buildcfg}"

include "GlacierV2.lua"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    debugdir "%{wks.location}"

    files
    {
        "Sandbox/src/**.h",
        "Sandbox/src/**.cpp"
    }

    links
    { 
        "GlacierV2"
    }

    includedirs 
    { 
        IncludeDirs["GlacierV2"],
        IncludeDirs["glfw"],
        IncludeDirs["glad"],
        IncludeDirs["glm"],
        IncludeDirs["entt"],
        "Sandbox/src/",
        "Sandbox/src/Prefabs",
        "Sandbox/src/Scenes",
        "Sandbox/src/Scripts"
    }

    defines 
    {
        GLMDefines,
        "_SHOW_CONSOLE" 
    }

    filter { "platforms:x64" }
        architecture "x64"
    
    filter "configurations:Debug_GL or Debug_DX"
        defines { "DEBUG" }
        symbols "On"
    
    filter "configurations:Release_GL or Release_DX"
        defines { "NDEBUG" }
        optimize "On"

    filter "configurations:Debug_DX or Release_DX"
        defines 
        { 
            "GLACIER_DIRECTX",
            "GLM_FORCE_DEPTH_ZERO_TO_ONE"
        }
    
    filter "configurations:Debug_GL or Release_GL"
        defines 
        { 
            "GLACIER_OPENGL"
        }