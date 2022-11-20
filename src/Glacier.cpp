#include "gpch.h"
#include "Glacier.h"
#include "Window.h"
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

Glacier* Glacier::instance = nullptr;

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	source, type, id, length; userParam;
	if (severity == GL_DEBUG_SEVERITY_HIGH || severity == GL_DEBUG_SEVERITY_MEDIUM)
	{
		OutputDebugStringA(message);
		OutputDebugStringA("\n");
	}
}
Glacier::Glacier()
{
	window = new Window(1920, 1080);

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
}
Glacier::~Glacier()
{
	delete window;
}

void Glacier::initialize_OpenGL()
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
void Glacier::run()
{
	// Initialize some singletons
	Renderer2DAtt::Initialize();
	RendererAtt::Initialize();
	SceneManagerAtt::Engine::Initialize();
	EditorLayer::Initialize();

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
	RendererAtt::Terminate();
	Renderer2DAtt::Terminate();
	TimeManagerAtt::Terminate();
	ShaderLoaderAtt::Terminate();
	ModelLoaderAtt::Terminate();
	TextureLoaderAtt::Terminate();
	FontLoaderAtt::Terminate();
	SkeletalAnimationLoaderAtt::Terminate();
}

void Glacier::Run()
{
	instance = new Glacier;	
	instance->run();
}
const Window& Glacier::GetWindow()
{
	assert(instance && "Glacier instance not created!");
	return *instance->window;
}
void Glacier::Terminate()
{
	delete instance;
	instance = nullptr;
}