#ifndef _FONT_LOADER
#define _FONT_LOADER

#include "GlacierCore.h"
#include "Font.h"

class GLACIER_API FontLoader
{
private:
	static const std::string FONT_PATH;

	static FontLoader* instance;
	static FontLoader& Instance()
	{
		if (!instance)
			instance = new FontLoader;
		return *instance;
	}
	FontLoader() = default;
	FontLoader(const FontLoader&) = delete;
	FontLoader& operator=(const FontLoader&) = delete;
	FontLoader(FontLoader&&) = delete;
	FontLoader& operator=(FontLoader&&) = delete;
	~FontLoader() = default;

	std::unordered_map<std::string, Font> fonts;

	void load(const std::string& name, const std::string& file_name, const int& size);
	const Font& get(const std::string& name);

	static void Terminate();

	friend class FontLoaderAtt;

public:
	static void Load(const std::string& name, const std::string& file_name, const int& size = 12) { Instance().load(name, file_name, size); }
	static const Font& Get(const std::string& name) { return Instance().get(name); }
};

#endif _FONT_LOADER