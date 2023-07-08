#ifndef _GRAPHICS_CONTEXT
#define _GRAPHICS_CONTEXT

#if GLACIER_OPENGL

#define WindowContext WindowOpenGL
#define InitializeGraphicsContext(window) GL::InitializeOpenGL(window)
#define TerminateGraphicsContext void

#define ShaderContext ShaderOpenGL
#define ShaderLocation "assets/shaders/"

#define ModelContext ModelOpenGL

#define TextureContext TextureOpenGL

#elif GLACIER_DIRECTX

#define WindowContext WindowDirectX
#define InitializeGraphicsContext(window) DX::Initialize(window)
#define TerminateGraphicsContext DX::Terminate

#define ShaderContext ShaderDirectX
#define ShaderLocation "assets/shaders/dx11/"

#define ModelContext ModelDirectX

#define TextureContext TextureDirectX

#endif

#endif