#include "gpch.h"
#include "SkeletalAnimationComponent.h"

SkeletalAnimationComponent::SkeletalAnimationComponent(const SkeletalAnimation* const anim, const float& playback_speed)
	: anim(anim),
	playback_time(0.0f), playback_speed(playback_speed)
{}
SkeletalAnimationComponent::SkeletalAnimationComponent(SkeletalAnimationComponent&& o)
	: anim(o.anim), playback_time(o.playback_time), playback_speed(o.playback_speed)
{
	o.anim = nullptr;
}
SkeletalAnimationComponent& SkeletalAnimationComponent::operator=(SkeletalAnimationComponent&& o)
{
	anim = o.anim;
	playback_time = o.playback_time;
	playback_speed = o.playback_speed;

	o.anim = nullptr;

	return *this;
}