#include "gpch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Script.h"
#include "RendererAtt.h"
#include "Renderer2DAtt.h"
#include "Lighting.h"
#include "Animator.h"
#include "Physics.h"
#include "TimeManager.h"
#include "Logger.h"

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
	GLACIER_LOG_FUNC_TIMER("update");
	scene_change->Execute();

	assert(curr_scene);
	Scene& scn = *curr_scene;

	scn.cmd_broker.DequeueAndExecuteAll();
	Physics::SimulatePhysics(TimeManager::GetDeltaTime(), 2, 1.0f / 60.0f, scn);
	Script::ExecuteAllOnUpdate(scn);
	scn.scn_graph.UpdateTransforms();
	Animator::UpdateAnimationComponents(scn);
	Lighting::UpdateBuffers(scn);
}
void SceneManager::renderCurrentScene()
{
	GLACIER_LOG_FUNC_TIMER("draw");
	assert(curr_scene);

	RendererAtt::RenderScene(*curr_scene);
	Renderer2DAtt::RenderComponents(*curr_scene);
}
void SceneManager::screenSizeChanged(const int& width, const int& height)
{
	Renderer2DAtt::UpdateViewportSize(width, height);
#if GLACIER_OPENGL
	RendererAtt::UpdateViewportSize(width, height);
#endif
	Script::ExecuteAllOnScreenResize(*curr_scene, width, height);
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
	//Script::ExecuteAllOnSceneEnter(*curr_scene);
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