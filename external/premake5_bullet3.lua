project "LinearMath"
    location "bullet3/src/LinearMath"
    kind "StaticLib"
    language "C++"
    warnings "Default"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "bullet3/src/LinearMath/**.h",
        "bullet3/src/LinearMath/**.cpp"
    }

    includedirs 
    {
        "bullet3/src/LinearMath",
        "bullet3/src"
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

project "BulletCollision"
    location "bullet3/src/BulletCollision"
    kind "StaticLib"
    language "C++"
    warnings "Default"
    
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "bullet3/src/BulletCollision/**.h",
        "bullet3/src/BulletCollision/**.cpp"
    }

    includedirs 
    {
        "bullet3/src/BulletCollision",
        "bullet3/src"
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

project "BulletDynamics"
    location "bullet3/src/BulletDynamics"
    kind "StaticLib"
    language "C++"
    warnings "Default"
    
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "bullet3/src/BulletDynamics/**.h",
        "bullet3/src/BulletDynamics/**.cpp"
    }

    includedirs 
    {
        "bullet3/src/BulletDynamics",
        "bullet3/src"
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