#ifndef _LIGHTING
#define _LIGHTING

#define DIR_SHADOW_MAP_SIZE 4096

class Scene;
struct DirectionalLightComponent;
struct CameraComponent;
class ConstantBuffer;

class Lighting
{
private:
	static Lighting* instance;
	static Lighting& Instance()
	{
		assert(instance && "Lighting not initialized!");
		return *instance;
	}
	Lighting();
	Lighting(const Lighting&) = delete;
	Lighting& operator=(const Lighting&) = delete;
	Lighting(Lighting&&) = delete;
	Lighting& operator=(Lighting&&) = delete;
	~Lighting();

	static const DirectionalLightComponent default_dir_light;
#if GLACIER_OPENGL
	GLuint DirLight_ubo;
	GLuint LightspaceMatrices_ubo;

	GLuint DirShadow_fbo;
	GLuint DirShadow_tex;
#elif GLACIER_DIRECTX
	ConstantBuffer* directionalLightCBuffer;
	ConstantBuffer* lightspaceMatrixCBuffer;

	ID3D11RenderTargetView* shadowRenderTargetView;
	ID3D11DepthStencilView* shadowDepthStencilView;
	ID3D11ShaderResourceView* shadowShaderResourceView;
	ID3D11SamplerState* shadowSamplerState;
#endif

	void renderSceneShadows(Scene* const curr_scene, const CameraComponent& cam);
	void updateBuffers(const Scene& curr_scene);

public:
	static void Initialize();
	static void Terminate();

	static void RenderSceneShadows(Scene* const curr_scene, const CameraComponent& cam) { Instance().renderSceneShadows(curr_scene, cam); }
	static void UpdateBuffers(const Scene& curr_scene) { Instance().updateBuffers(curr_scene); }

#if GLACIER_OPENGL
	static GLuint GetDirLightUBO() { return Instance().DirLight_ubo; }
	static GLuint GetLightspaceMatricesUBO() { return Instance().LightspaceMatrices_ubo; }
#elif GLACIER_DIRECTX
	static ConstantBuffer* const GetDirectionalLightConstantBuffer() { return instance->directionalLightCBuffer; }
	static ConstantBuffer* const GetLightspaceMatrixConstantBuffer() { return instance->lightspaceMatrixCBuffer; }
#endif

	friend class SceneManager;
	friend class ShaderLoader;
	friend class Renderer;
};

#endif _LIGHTING