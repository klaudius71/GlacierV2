#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#include <cassert>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <future>
#include <mutex>

#include "GlacierCore.h"
#include "VertexTypes.h"
#include "WeakPtr.h"
#include "GMathTools.h"
#include "Colors.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dcommon.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_INTRINSICS
#define GLM_FORCE_SSE42
#define GLM_FORCE_INLINE
#define GLM_ENABLE_EXPERIMENTAL // glm::length2() is experimental for some reason
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "entt/entity/registry.hpp"

//#include "imgui.h"

#include "Application.h"
#include "Window.h"
#include "ShaderLoader.h"
#include "Shader.h"
#include "ModelLoader.h"
#include "Model.h"
#include "TextureLoader.h"
#include "Texture.h"
#include "SkeletalAnimationLoader.h"
#include "SkeletalAnimation.h"
#include "FontLoader.h"
#include "Font.h"
#include "TimeManager.h"
#include "Timer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Entity.h"
#include "Script.h"
#include "CameraControllerScript.h"
#include "Input.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Prefab.h"
#include "Logger.h"