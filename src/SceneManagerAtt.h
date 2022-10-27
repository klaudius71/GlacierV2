#ifndef _SCENE_MANAGER_ATT
#define _SCENE_MANAGER_ATT

#include "SceneManager.h"

struct SceneManagerAtt
{
	class Engine 
	{
		friend class Glacier;

		static void Initialize() { SceneManager::Initialize(); }
		static void UpdateCurrentScene() { SceneManager::UpdateCurrentScene(); }
		static void RenderCurrentScene() { SceneManager::RenderCurrentScene(); }
		static void Terminate() { SceneManager::Terminate(); }
	};

	class SceneChange
	{
		friend class SceneChangeCmd;

		static void ChangeScene(Scene* const scn) { SceneManager::ChangeScene(scn); }
	};

	class Callback
	{
		friend class EditorLayer;

		static void ScreenSizeChanged(const int& width, const int& height) { SceneManager::ScreenSizeChanged(width, height); }
	};
};	

#endif _SCENE_MANAGER_ATT