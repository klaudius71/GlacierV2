#pragma once

#if _WIN32
#include <Windows.h>
#endif

#include <cassert>
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

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_INTRINSICS
#define GLM_FORCE_SSE42
#define GLM_FORCE_INLINE
#include "glm/glm.hpp"
#include "glm/ext.hpp"

#include "entt/entity/registry.hpp"

#include "json.hpp"
#include "base64.h"

#include "Colors.h"

//#pragma warning(disable : 4005)
//#include "Framework.h"
