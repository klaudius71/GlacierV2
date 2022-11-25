#ifndef _GLACIER
#define _GLACIER

#define SHOW_EDITOR

class Window;

class Glacier
{
private:
	static Glacier* instance;
	Glacier();
	Glacier(const Glacier&) = delete;
	Glacier& operator=(const Glacier&) = delete;
	Glacier(Glacier&&) = delete;
	Glacier& operator=(Glacier&&) = delete;
	~Glacier();

	void initialize_OpenGL();
	void run();
	
	// User code
	// Load all assets here
	void LoadResources();

private:
	Window* window = nullptr;

public:
	static void Run();
	static const Window& GetWindow();
	static void Terminate();
};

#endif _GLACIER
