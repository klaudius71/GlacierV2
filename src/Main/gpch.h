#pragma once

#if _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#define UNREFERENCED_PARAMETER(x) (x)
#endif

#include <cassert>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <future>
#include <mutex>
#include <functional>
#include "Timer.h"
#include "VertexTypes.h"
#include "WeakPtr.h"
#include "GMathTools.h"

#if GLACIER_DIRECTX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dcommon.h>
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#endif

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#if _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include "GLFW/glfw3native.h"

#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "entt/entity/registry.hpp"

#include "json.hpp"
#include "base64.h"

#include "ft2build.h"
#include FT_FREETYPE_H

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "btBulletDynamicsCommon.h"

#include "Colors.h"

//#pragma warning(disable : 4005)
//#include "../external/MemTracker/Framework.h"