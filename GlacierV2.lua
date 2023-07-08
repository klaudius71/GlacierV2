workspace "GlacierV2"
    configurations { "Debug_GL", "Release_GL", "Debug_DX", "Release_DX" }
    platforms { "x64" }
    startproject "Sandbox"
    flags { "MultiProcessorCompile" }
    warnings "Extra"

include "premake_common.lua"

group "Dependencies"
    include "external"
group "Dependencies/BulletPhysics"
    include "external/premake5_bullet3.lua"
group ""

project "GlacierV2"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "On"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "gpch.h"
    pchsource "src/Main/gpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    links 
    { 
        "GLFW",
        "glad",
        "imgui",
        "base64",
        "LinearMath",
        "BulletCollision",
        "BulletDynamics"
    }

    includedirs 
    { 
        IncludeDirs["GlacierV2"],
        IncludeDirs["glfw"],
        IncludeDirs["glad"],
        IncludeDirs["glm"],
        IncludeDirs["entt"],
        IncludeDirs["imgui"],
        IncludeDirs["stb"],
        IncludeDirs["JSON for Modern C++"],
        IncludeDirs["base64"],
        IncludeDirs["FreeType"],
        IncludeDirs["Bullet"]
    }

    filter "platforms:x64"
        architecture "x64"

    filter "configurations:Debug_GL or Debug_DX"
        defines { "DEBUG" }
        symbols "On"
        links 
        {
            "%{wks.location}/external/freetype/lib/x64/debug/freetype.lib"
        }
    
    filter "configurations:Release_GL or Release_DX"
        defines { "NDEBUG" }
        optimize "On"
        links 
        {
            "%{wks.location}/external/freetype/lib/x64/release/freetype.lib"
        }

    filter "configurations:Debug_GL or Release_GL"
        defines 
        { 
            "GLACIER_OPENGL"
        }
    
    filter "configurations:Debug_DX or Release_DX"
        defines 
        { 
            "GLACIER_DIRECTX",
            "GLM_FORCE_DEPTH_ZERO_TO_ONE"
        }