#ifndef _SCENE_MANAGER
#define _SCENE_MANAGER

#include "SceneChangeCmd.h"
#include "SceneChangeNullCmd.h"

class Scene;
class Command;

class SceneManager
{
private:
	static SceneManager* instance;
	SceneManager();
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator=(SceneManager&&) = delete;
	~SceneManager();

	Scene* curr_scene;
	SceneChangeCmd* scene_change;
	SceneChangeCmd scene_change_cmd;
	SceneChangeNullCmd scene_change_cmd_null;

	void updateCurrentScene();
	void renderCurrentScene();
	void screenSizeChanged(const int& width, const int& height);
	void enqueueCommand(Command* const cmd);
	void setNextScene(Scene* const next_scene);
	void changeScene(Scene* const next_scene);

	static void Initialize();
	static void ChangeScene(Scene* const scn) { instance->changeScene(scn); }
	static void UpdateCurrentScene() { instance->updateCurrentScene(); }
	static void RenderCurrentScene() { instance->renderCurrentScene(); }
	static void ScreenSizeChanged(const int& width, const int& height) { instance->screenSizeChanged(width, height); }
	static void Terminate();

	friend struct SceneManagerAtt;
public:
	static void SetNextScene(Scene* const next_scene) { instance->setNextScene(next_scene); }
	static void EnqueueCommand(Command* const cmd) { instance->enqueueCommand(cmd); }
	static Scene* const GetCurrentScene();
};

#endif _SCENE_MANAGER
