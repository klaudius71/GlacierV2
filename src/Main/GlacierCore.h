#pragma once

#ifdef GLACIER_DLL
#ifdef GLACIER_BUILD_DLL
	#define GLACIER_API __declspec(dllexport)
#else
	#define GLACIER_API __declspec(dllimport)
#endif // GLACIER_BUILD_DLL
#else
	#define GLACIER_API
#endif // GLACIER_DLL

#define ALIGN16 alignas(16)

#ifdef _WIN32
#define NATIVE_WINDOW HWND
#else
#define NATIVE_WINDOW int
#endif