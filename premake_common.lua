-- Includes
IncludeDirs = {}

--- External includes
IncludeDirs["glfw"] = "%{wks.location}/external/glfw/include"
IncludeDirs["glad"] = "%{wks.location}/external/glad/include"
IncludeDirs["glm"] = "%{wks.location}/external/glm"
IncludeDirs["entt"] = "%{wks.location}/external/entt-3.10.3/src"
IncludeDirs["imgui"] = "%{wks.location}/external/imgui"
IncludeDirs["stb"] = "%{wks.location}/external/stb"
IncludeDirs["JSON for Modern C++"] = "%{wks.location}/external/JSON for Modern C++"
IncludeDirs["base64"] = "%{wks.location}/external/base64/include"
IncludeDirs["FreeType"] = "%{wks.location}/external/freetype/include"
IncludeDirs["Bullet"] = "%{wks.location}/external/bullet3/src"

-- Glacier includes
IncludeDirs["GlacierV2"] = 
{
    "%{wks.location}/src",
    "%{wks.location}/src/Asset",
    "%{wks.location}/src/Asset/DirectX",
    "%{wks.location}/src/Asset/OpenGL",
    "%{wks.location}/src/Asset/Loaders",
    "%{wks.location}/src/Asset/SkeletalAnimation",
    "%{wks.location}/src/Attorneys",
    "%{wks.location}/src/ECS",
    "%{wks.location}/src/ECS/Components",
    "%{wks.location}/src/GraphicsContext",
    "%{wks.location}/src/GraphicsContext/DirectX",
    "%{wks.location}/src/GraphicsContext/OpenGL",
    "%{wks.location}/src/Main",
    "%{wks.location}/src/Misc",
    "%{wks.location}/src/Misc/Scripts",
    "%{wks.location}/src/Misc/Tools",
    "%{wks.location}/src/Systems",
    "%{wks.location}/src/Systems/Command",
    "%{wks.location}/src/Systems/Command/Commands",
    "%{wks.location}/src/Systems/Command/Commands/Entity",
    "%{wks.location}/src/Systems/Command/Commands/Scene",
    "%{wks.location}/src/Systems/Editor",
    "%{wks.location}/src/Systems/Editor/ShowEditor",
    "%{wks.location}/src/Systems/Input",
    "%{wks.location}/src/Systems/Lighting",
    "%{wks.location}/src/Systems/Physics",
    "%{wks.location}/src/Systems/Render",
    "%{wks.location}/src/Systems/Render/Skeletal",
    "%{wks.location}/src/Systems/Scene",
    "%{wks.location}/src/Systems/Scripting",
    "%{wks.location}/src/Time",
}

-- Libraries
LibraryDirs = {}

-- GLM defines
GLMDefines = 
{
    "GLM_FORCE_SILENT_WARNINGS",
    --"GLM_FORCE_DEFAULT_ALIGNED_GENTYPES", -- Lots of fixing needed to get this working
    "GLM_FORCE_SSE42",
    "GLM_ENABLE_EXPERIMENTAL",
}