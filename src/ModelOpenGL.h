#ifndef _MODEL_OPENGL
#define _MODEL_OPENGL

#include "Model.h"

#if GLACIER_OPENGL

class GLACIER_API ModelOpenGL : public Model
{
public:
	template<typename... Args>
	ModelOpenGL(Args&&... args)
		: Model(std::forward<Args>(args)...)
	{
	}
	ModelOpenGL(const ModelOpenGL&) = delete;
	ModelOpenGL& operator=(const ModelOpenGL&) = delete;
	ModelOpenGL(ModelOpenGL&& o) noexcept;
	ModelOpenGL& operator=(ModelOpenGL&& o);
	~ModelOpenGL();

	virtual void Bind() const override;

	const GLuint GetVAO() const;
	const GLuint GetVBO() const;
	const GLuint GetEBO() const;

private:
	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ebo = 0;

	virtual void load_gpu_data() override;
};

#endif

#endif