#include "gpch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Script.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Lighting.h"

SceneManager* SceneManager::instance = nullptr;

SceneManager::SceneManager()
	: curr_scene(new Scene), scene_change(&scene_change_cmd_null)
{}
SceneManager::~SceneManager()
{
	curr_scene->EndScene();
	delete curr_scene;
}

void SceneManager::updateCurrentScene()
{
	scene_change->Execute();

	assert(curr_scene);

	curr_scene->cmd_broker.DequeueAndExecuteAll();
	Script::ExecuteAllOnTick(*curr_scene);
	curr_scene->scn_graph.UpdateTransforms();
	Lighting::UpdateBuffers(*curr_scene);
}
void SceneManager::renderCurrentScene()
{
	assert(curr_scene);

	Renderer::RenderScene(curr_scene);
	Renderer2D::RenderComponents(curr_scene->GetRegistry());
}
void SceneManager::enqueueCommand(Command* const cmd)
{
	curr_scene->cmd_broker.Enqueue(cmd);
}
void SceneManager::setNextScene(Scene* const next_scene)
{
	assert(next_scene);

	scene_change_cmd.SetScene(next_scene);
	scene_change = &scene_change_cmd;
}
void SceneManager::changeScene(Scene* const next_scene)
{
	assert(next_scene);
	scene_change = &scene_change_cmd_null;
	curr_scene->EndScene();
	delete curr_scene;
	curr_scene = next_scene;
	curr_scene->InitializeScene();
}

Scene* const SceneManager::GetCurrentScene()
{
	assert(instance && "SceneManager not initialized yet!");
	return instance->curr_scene;
}

void SceneManager::Initialize()
{
	assert(!instance && "SceneManager already initialized!");
	instance = new SceneManager;
}
void SceneManager::Terminate()
{
	delete instance;
	instance = nullptr;
}