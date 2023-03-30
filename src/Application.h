#ifndef _GLACIER
#define _GLACIER

//#define SHOW_EDITOR

#include "GlacierCore.h"

class Window;

namespace Glacier {

	class GLACIER_API Application
	{
	private:
		static Application* instance;
	protected:
		Application(const int window_width = 1600, const int window_height = 900, const char* const icon_path = nullptr);
		virtual ~Application();
	private:
		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;
		Application(Application&&) = delete;
		Application& operator=(Application&&) = delete;
	
		void initialize_OpenGL();

	public:
		void Run();
		
		// User code
		// Load all assets here
		virtual void LoadResources() = 0;
	
	private:
		Window* window = nullptr;
	
	public:
		static const Window& GetWindow();
	};

	const GLACIER_API Window& GetWindow();
}


#endif _GLACIER
