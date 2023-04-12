#ifndef _GL
#define _GL

class Window;

class GL
{
public:
	static void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userParam);
	static void InitializeOpenGL(Window* const window);
};

#endif
