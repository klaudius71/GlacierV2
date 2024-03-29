#pragma once

#ifdef GLACIER_BUILD_DLL
	#define GLACIER_API __declspec(dllexport)
#else
	#define GLACIER_API __declspec(dllimport)
#endif

#define ALIGN16 alignas(16)

#ifdef _WIN32
#define NATIVE_WINDOW HWND
#else
#define NATIVE_WINDOW int
#endif