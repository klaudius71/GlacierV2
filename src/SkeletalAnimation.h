#ifndef _SKELETAL_ANIMATION
#define _SKELETAL_ANIMATION

class Bone;

class SkeletalAnimation
{
public:
	SkeletalAnimation() = delete;
	SkeletalAnimation(const std::string& file_name);
	SkeletalAnimation(const SkeletalAnimation&) = delete;
	SkeletalAnimation& operator=(const SkeletalAnimation&) = delete;
	SkeletalAnimation(SkeletalAnimation&& o);
	SkeletalAnimation& operator=(SkeletalAnimation&& o);
	~SkeletalAnimation();

	void Apply(std::vector<glm::mat4>& mats, const float& timestamp) const;

	const std::string& GetName() const;
	const Bone& GetBoneHierarchy() const;
	const float& GetAnimationDuration() const;

private:
	std::string name;
	Bone* bone_hierarchy_root;
	float anim_duration;
};

#endif _SKELETAL_ANIMATION