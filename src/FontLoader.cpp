#include "gpch.h"
#include "FontLoader.h"

FontLoader* FontLoader::instance = nullptr;
const std::string FontLoader::FONT_PATH = "fonts/";

void FontLoader::load(const std::string& name, const std::string& file_name, const int& size)
{
	assert(fonts.find(name) == fonts.cend() && "Attempted to load a duplicate font!");
	fonts.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(FONT_PATH + file_name, size));
}

const Font& FontLoader::get(const std::string& name)
{
	auto it = fonts.find(name);
	assert(it != fonts.cend());
	return it->second;
}

void FontLoader::Terminate()
{
	delete instance;
	instance = 0;
}
