#include "gpch.h"
#include "Glacier.h"
#include "Window.h"
#include "ShaderLoaderAtt.h"
#include "TextureLoaderAtt.h"
#include "ModelLoaderAtt.h"
#include "FontLoaderAtt.h"
#include "TimeManagerAtt.h"
#include "SceneManagerAtt.h"
#include "Scene.h"
#include "Renderer2DAtt.h"
#include "InputAtt.h"

Glacier* Glacier::instance = nullptr;

void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam)
{
	source, type, id, severity; length; userParam;

	OutputDebugStringA(message);
	OutputDebugStringA("\n");
}
Glacier::Glacier()
{
	window = new Window;

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
	TimeManagerAtt::Initialize();
	Renderer2DAtt::Initialize();
	SceneManagerAtt::Engine::Initialize();

	// Loads user defined assets
	LoadResources();
	ShaderLoaderAtt::Initialize(); // Fallback for if the user did not load their own shaders
	ModelLoaderAtt::WaitForThreadsAndLoadGPUData();
	TextureLoaderAtt::WaitForThreadsAndLoadGPUData();

	// Running all script OnCreate()'s
	SceneManagerAtt::Engine::UpdateCurrentScene();

	// Main loop
	while (!window->IsOpen())
	{
		window->Clear();
	
		TimeManagerAtt::ProcessTime();
		InputAtt::ProcessMouseData();
	
		// update
		SceneManagerAtt::Engine::UpdateCurrentScene();
	
		// render
		SceneManagerAtt::Engine::RenderCurrentScene();
		window->SwapBuffers();

		TimeManagerAtt::LimitFrametime(1.0f / 240.5f);
		window->PollEvents();
	}

	// Terminate all of the singletons
	SceneManagerAtt::Engine::Terminate();
	Renderer2DAtt::Terminate();
	TimeManagerAtt::Terminate();
	ShaderLoaderAtt::Terminate();
	ModelLoaderAtt::Terminate();
	TextureLoaderAtt::Terminate();
	FontLoaderAtt::Terminate();
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