#include "gpch.h"
#include "Application.h"
#include "GraphicsContext.h"
#include "WindowDirectX.h"
#include "WindowOpenGL.h"
#include "ShaderLoaderAtt.h"
#include "TextureLoaderAtt.h"
#include "ModelLoaderAtt.h"
#include "FontLoaderAtt.h"
#include "SkeletalAnimationLoaderAtt.h"
#include "TimeManagerAtt.h"
#include "SceneManagerAtt.h"
#include "Scene.h"
#include "Renderer2DAtt.h"
#include "RendererAtt.h"
#include "InputAtt.h"
#include "EditorLayer.h"
#include "Physics.h"
#include "LoggerAtt.h"
#include "UUIDAtt.h"
#include "Lighting.h"
#if GLACIER_OPENGL
#include "GL.h"
#elif GLACIER_DIRECTX
#include "DX.h"
#endif

namespace Glacier {

	Application* Application::instance = nullptr;

	Application::Application(const int window_width, const int window_height, const char* const icon_path)
	{
		assert(instance == nullptr);
		instance = this;

		window = new WindowContext(window_width, window_height, icon_path);
	}
	Application::~Application()
	{
		delete window;
	}

	void Application::Run()
	{
		// Initialize some singletons
		InitializeGraphicsContext(window);
		Renderer2DAtt::Initialize();
		RendererAtt::Initialize();
		Lighting::Initialize();
		SceneManagerAtt::Engine::Initialize();
		EditorLayer::Initialize();
		Physics::Initialize();
		LoggerAtt::Initialize();
		Tools::UUIDAtt::Initialize();

		// Loads user defined assets
		LoadResources();
		ShaderLoaderAtt::Initialize(); // Fallback for if the user did not load their own shaders
		ModelLoaderAtt::WaitForThreadsAndLoadGPUData();
		TextureLoaderAtt::WaitForThreadsAndLoadGPUData();
		SkeletalAnimationLoaderAtt::WaitForThreads();

		// This one's later
		TimeManagerAtt::Initialize();

		// Main loop
		while (!window->IsOpen())
		{
			GLACIER_LOG_FUNC_TIMER("frame_time");

			window->Clear();
#if GLACIER_OPENGL
			Renderer::GetMainFramebuffer().Clear();
#endif

			TimeManagerAtt::ProcessTime();
			InputAtt::ProcessMouseData();

			EditorLayer::NewFrame();

			// update
			SceneManagerAtt::Engine::UpdateCurrentScene();
	
			// render
			SceneManagerAtt::Engine::RenderCurrentScene();

			EditorLayer::ShowEditor();
			EditorLayer::Render();

			window->SwapBuffers();

			//TimeManagerAtt::LimitFrametime(1.0f / 240.5f);
			window->PollEvents();
		}

#if GLACIER_OPENGL
		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
#endif

		// Terminate all of the singletons
		EditorLayer::Terminate();
		SceneManagerAtt::Engine::Terminate();
		Tools::UUIDAtt::Terminate();
		LoggerAtt::Terminate();
		Physics::Terminate();
		Lighting::Terminate();
		RendererAtt::Terminate();
		Renderer2DAtt::Terminate();
		TimeManagerAtt::Terminate();
		ShaderLoaderAtt::Terminate();
		ModelLoaderAtt::Terminate();
		TextureLoaderAtt::Terminate();
		FontLoaderAtt::Terminate();
		SkeletalAnimationLoaderAtt::Terminate();
		TerminateGraphicsContext();
	}

	const Window& Application::GetWindow()
	{
		assert(instance && "Application instance not set!");
		return *instance->window;
	}

	const Window& GetWindow() 
	{
		return Application::GetWindow();
	}
}