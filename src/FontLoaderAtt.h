#ifndef _FONT_LOADER_ATT
#define _FONT_LOADER_ATT

#include "FontLoader.h"

class FontLoaderAtt
{
	friend class Glacier;

	static void Terminate() { FontLoader::Terminate(); }
};

#endif _FONT_LOADER_ATT