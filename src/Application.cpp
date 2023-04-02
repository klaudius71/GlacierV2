#include "gpch.h"
#include "Application.h"
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
#if GLACIER_DIRECTX
#include "DX.h"
#endif

namespace Glacier {

	Application* Application::instance = nullptr;

	void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
	{
		source, type, id, length; userParam;
		if (severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM)
		{
			OutputDebugStringA(message);
			OutputDebugStringA("\n");
		}
	}
	Application::Application(const int window_width, const int window_height, const char* const icon_path)
	{
		assert(instance == nullptr);
		instance = this;

#if GLACIER_OPENGL
		window = new WindowOpenGL(window_width, window_height, icon_path);

		// Initializes GLAD
		initialize_OpenGL();

	#if _DEBUG
		int flags; 
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
		}
	#endif

		glClearColor(0.471f, 0.694f, 0.749f, 1.0f);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glEnable(GL_DEPTH_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#elif GLACIER_DIRECTX
		window = new WindowDirectX(window_width, window_height, icon_path);
#endif
	}
	Application::~Application()
	{
		delete window;
	}

	void Application::initialize_OpenGL()
	{
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			delete window;
			assert(false && "Failed to initialize GLAD!");
		}

		glViewport(0, 0, window->GetWindowWidth(), window->GetWindowHeight());
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	void Application::Run()
	{
#if GLACIER_OPENGL
		// Initialize some singletons
		Renderer2DAtt::Initialize();
		RendererAtt::Initialize();
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
			Renderer::GetMainFramebuffer().Clear();

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

		// Cleanup
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		// Terminate all of the singletons
		EditorLayer::Terminate();
		SceneManagerAtt::Engine::Terminate();
		Tools::UUIDAtt::Terminate();
		LoggerAtt::Terminate();
		Physics::Terminate();
		RendererAtt::Terminate();
		Renderer2DAtt::Terminate();
		TimeManagerAtt::Terminate();
		ShaderLoaderAtt::Terminate();
		ModelLoaderAtt::Terminate();
		TextureLoaderAtt::Terminate();
		FontLoaderAtt::Terminate();
		SkeletalAnimationLoaderAtt::Terminate();
#elif GLACIER_DIRECTX
		DX::Initialize(*window);
		Renderer2DAtt::Initialize();
		//RendererAtt::Initialize();
		SceneManagerAtt::Engine::Initialize();
		//EditorLayer::Initialize();
		Physics::Initialize();
		LoggerAtt::Initialize();
		Tools::UUIDAtt::Initialize();

		// Loads user defined assets
		LoadResources();
		ShaderLoaderAtt::Initialize(); // Fallback for if the user did not load their own shaders
		ModelLoaderAtt::WaitForThreadsAndLoadGPUData();
		TextureLoaderAtt::WaitForThreadsAndLoadGPUData();
		SkeletalAnimationLoaderAtt::WaitForThreads();

		TimeManagerAtt::Initialize();

		ID3D11DeviceContext* context = DX::GetDeviceContext();
		//TextureDirectX* tex = new TextureDirectX("assets/textures/crate_diffuse.tga", TextureParameters());
		const Model* cube = ModelLoader::Get(PRELOADED_MODELS::UNIT_CUBE);
		const Shader* shad = ShaderLoader::Get(PRELOADED_SHADERS::TEXTURE);

		auto matrixCBuffer = ShaderLoader::GetCamDataConstantBuffer();
		auto instanceCBuffer = ShaderLoader::GetInstanceDataConstantBuffer();

		const glm::mat4 cam_matrices[] = {
			glm::perspective(glm::radians(90.0f), 16.0f / 9.0f, 0.1f, 1000.0f),
			//glm::ortho(0.0f, (float)window->GetWindowWidth(), 0.0f, (float)window->GetWindowHeight()),
			glm::lookAt(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f))
		};
		context->UpdateSubresource(matrixCBuffer, 0, nullptr, cam_matrices, 0, 0);

		glm::mat4 world = glm::scale(glm::vec3(10.0f, 10.0f, 10.0f));
		context->UpdateSubresource(instanceCBuffer, 0, nullptr, &world, 0, 0);

		// Main loop
		while (!window->IsOpen())
		{
			/*window->Clear();

			TimeManagerAtt::ProcessTime();

			world *= glm::rotate(TimeManager::GetDeltaTime(), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
			world *= glm::rotate(0.5f * TimeManager::GetDeltaTime(), glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
			world *= glm::rotate(0.25f * TimeManager::GetDeltaTime(), glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f)));
			context->UpdateSubresource(instanceCBuffer, 0, nullptr, &world, 0, 0);

			shad->Bind();
			tex->Bind();
			cube->Bind();
			context->DrawIndexed(cube->GetNumTriangles() * 3, 0, 0);

			window->SwapBuffers();
			window->PollEvents();*/

			GLACIER_LOG_FUNC_TIMER("frame_time");

			window->Clear();
			//Renderer::GetMainFramebuffer().Clear();

			TimeManagerAtt::ProcessTime();
			InputAtt::ProcessMouseData();

			// update
			SceneManagerAtt::Engine::UpdateCurrentScene();

			// render
			SceneManagerAtt::Engine::RenderCurrentScene();

			window->SwapBuffers();
			window->PollEvents();
		}
		
		//delete tex;

		ModelLoaderAtt::Terminate();
		ShaderLoaderAtt::Terminate();

		// Terminate all of the singletons
		SceneManagerAtt::Engine::Terminate();
		Tools::UUIDAtt::Terminate();
		LoggerAtt::Terminate();
		Physics::Terminate();
		//RendererAtt::Terminate();
		Renderer2DAtt::Terminate();
		TimeManagerAtt::Terminate();
		ShaderLoaderAtt::Terminate();
		ModelLoaderAtt::Terminate();
		TextureLoaderAtt::Terminate();
		FontLoaderAtt::Terminate();
		SkeletalAnimationLoaderAtt::Terminate();

		DX::Terminate();
#endif
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