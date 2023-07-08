#ifndef _SKELETAL_ANIMATION_LOADER_ATT
#define _SKELETAL_ANIMATION_LOADER_ATT

#include "SkeletalAnimationLoader.h"

class SkeletalAnimationLoaderAtt
{
	friend class Glacier::Application;

	static void WaitForThreads() { SkeletalAnimationLoader::WaitForThreads(); }
	static void Terminate() { SkeletalAnimationLoader::Terminate(); }
};

#endif _SKELETAL_ANIMATION_LOADER_ATT