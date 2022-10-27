#include "gpch.h"
#include "EditorLayer.h"
#include "Glacier.h"
#include "Window.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SceneAtt.h"
#include "GameObject.h"

EditorLayer* EditorLayer::instance = nullptr;

EditorLayer::EditorLayer()
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsLight();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(Glacier::GetWindow().GetNativeWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");
}

void EditorLayer::drawGraph(GameObjectRef go)
{
	const GameObject* const_go = go.getWeakPtr().lock().get();
	if (const_go->GetChildren().size() > 0)
	{
		if (ImGui::TreeNodeEx(go->GetComponent<NameComponent>().name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth))
		{
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				selected_go = go;

			for (const auto& children : const_go->GetChildren())
			{
				drawGraph(children);
			}
			ImGui::TreePop();
		}
	}
	else
	{
		if (ImGui::TreeNodeEx(go->GetComponent<NameComponent>().name.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_SpanAvailWidth))
		{
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
				selected_go = go;
		}
	}
}
void EditorLayer::showEditor()
{
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_::ImGuiDockNodeFlags_PassthruCentralNode);

	static bool show_demo_window = true;
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	Scene& current_scene = *SceneManager::GetCurrentScene();
	SceneGraph& scn_graph = SceneAtt::GetSceneGraph(current_scene);

	ImGui::Begin("Scene Graph");
	for (const auto& go : scn_graph.GetGraph())
	{
		if (!go->GetParent().isExpired())
			continue;

		drawGraph(go);
	}
	ImGui::End();

	ImGui::Begin("Properties");
	if (!selected_go.isExpired())
	{
		auto [camera, script, sprite, mesh, material, dir_light, skybox] = selected_go->TryGetComponent<CameraComponent, ScriptComponent, SpriteComponent, MeshComponent, MaterialComponent, DirectionalLightComponent, SkyboxComponent>();

		if (ImGui::TreeNode("Name Component"))
		{
			NameComponent& name_component = selected_go->GetComponent<NameComponent>();
			ImGui::Text("Name: %s", name_component.name.c_str());
			ImGui::Text("ID: %x", name_component.id);
			ImGui::TreePop();
		}

		if (ImGui::TreeNode("Transform Component"))
		{
			TransformComponent& transform_component = selected_go->GetComponent<TransformComponent>();
			ImGui::DragFloat3("Position", (float*)&transform_component.position());
			glm::vec3 to_degrees = glm::degrees(transform_component.rotation());
			ImGui::DragFloat3("Rotation", (float*)&to_degrees);
			transform_component.rotation() = glm::radians(to_degrees);
			ImGui::DragFloat3("Scale", (float*)&transform_component.scale());
			ImGui::TreePop();
		}

		if (camera && ImGui::TreeNode("Camera Component"))
		{
			ImGui::DragFloat3("Position", (float*)&camera->cam_pos);
			ImGui::DragFloat3("Direction", (float*)&camera->cam_dir);
			float fov_degrees = glm::degrees(camera->fov);
			ImGui::DragFloat("FOV", &fov_degrees);
			camera->fov = glm::radians(fov_degrees);
			ImGui::TreePop();
		}

		if (script && ImGui::TreeNode("Script Component"))
		{
			ImGui::Text("Script: %s", script->script->GetName().c_str());
			ImGui::TreePop();
		}

		if (sprite && ImGui::TreeNode("Sprite Component"))
		{
			ImGui::Text("Texture:");
			ImGui::Image((void*)(uint64_t)sprite->tex_id, ImVec2{ 256, 256 });
			if (ImGui::TreeNode("Texture Details"))
			{
				ImGui::Text("File Path: %s", sprite->tex->GetFilePath().c_str());
				ImGui::Text("Size:");
				ImGui::BulletText("Width: %d", sprite->tex->GetWidth());
				ImGui::BulletText("Height: %d", sprite->tex->GetHeight());
				ImGui::TreePop();
			}
			ImGui::TreePop();
		}

		if(mesh && ImGui::TreeNode("Mesh Component"))
		{
			ImGui::TreePop();
		}

		if (material && ImGui::TreeNode("Material Component"))
		{
			ImGui::TreePop();
		}

		if (dir_light && ImGui::TreeNode("Directional Light Component"))
		{
			ImGui::TreePop();
		}

		if (skybox && ImGui::TreeNode("Skybox Component"))
		{
			ImGui::TreePop();
		}
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}