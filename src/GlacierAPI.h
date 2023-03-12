#if _WIN32
#include <Windows.h>
#endif

#include <cassert>
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

#include "../external/glad/include/glad/glad.h"
#include "../external/glfw-3.3.8/include/GLFW/glfw3.h"

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