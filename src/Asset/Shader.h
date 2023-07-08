#ifndef _SHADER
#define _SHADER

#include "GlacierCore.h"

class GLACIER_API Shader
{
protected:
	Shader() = default;
	Shader(const Shader&) = delete;
	Shader& operator=(const Shader&) = delete;
	Shader(Shader&&) = delete;
	Shader& operator=(Shader&&) = delete;
	virtual ~Shader() = default;

public:
	virtual void Bind() const = 0;
};

#endif _SHADER