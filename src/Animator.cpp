#include "gpch.h"
#include "Animator.h"
#include "Scene.h"
#include "Components.h"
#include "TimeManager.h"
#include "SkeletalAnimation.h"
#include "Model.h"

void Animator::UpdateAnimationComponents(Scene& scn)
{
	GLACIER_LOG_FUNC_TIMER("animation");

	entt::registry& registry = scn.GetRegistry();

	const auto view = registry.view<SkeletalAnimationComponent, SkeletalMeshComponent>();
	for (auto&& [entity, anim, skel_mesh] : view.each())
	{
		anim.playback_time += TimeManager::GetDeltaTime() * anim.playback_speed;
		if (anim.playback_time > anim.anim->GetAnimationDuration())
			anim.playback_time -= anim.anim->GetAnimationDuration();

		if (anim.playback_speed != 0.0f)
			anim.anim->Apply(skel_mesh.bone_matrices, skel_mesh.mod->GetInverseBindMatrices(), anim.playback_time);
	}
}