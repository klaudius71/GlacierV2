#ifndef _SKELETAL_ANIMATION_COMPONENT
#define _SKELETAL_ANIMATION_COMPONENT

class SkeletalAnimation;

struct SkeletalAnimationComponent
{
	const SkeletalAnimation* anim;
	float playback_time;
	float playback_speed;

	SkeletalAnimationComponent(const SkeletalAnimation* const anim, const float& playback_speed = 1.0f);
	SkeletalAnimationComponent(SkeletalAnimationComponent&& o);
	SkeletalAnimationComponent& operator=(SkeletalAnimationComponent&& o);
};

#endif