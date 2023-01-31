#ifndef _GMathTools
#define _GMathTools

#include "GlacierCore.h"

class CollisionVolume;
class CollisionVolumeBSphere;
class CollisionVolumeAABB;
class CollisionVolumeBoundingBoxBase;
class CollisionVolumeOBB;

/** \brief Optional math tools
*	Some of these (mainly Collision) are used by engine subsystems.
*	\ingroup MISC
*/
namespace Tools
{
	namespace Collision
	{
		GLACIER_API inline bool NumInInterval(const float& num, const float& min, const float& max);
		GLACIER_API bool PointInAABB(const glm::vec3& p, const glm::vec3& min, const glm::vec3& max);
		GLACIER_API bool PointInOBB(const glm::vec3& p, const CollisionVolumeOBB& obb);
		GLACIER_API inline bool IntervalIntersect(const float& min0, const float& max0, const float& min1, const float& max1);
		GLACIER_API const float ClampNumToInterval(const float& num, const float& min, const float& max);
		GLACIER_API const glm::vec3 ClampPointToInterval(const glm::vec3& c, const glm::vec3& min, const glm::vec3& max); 

		GLACIER_API const float OBBMaxProjectionOnAxis(const CollisionVolumeBoundingBoxBase& obb, const glm::vec3& v);
		GLACIER_API bool TestOBBsOnAxis(const CollisionVolumeBoundingBoxBase& a, const CollisionVolumeBoundingBoxBase& b, const glm::vec3& v);

		GLACIER_API bool Intersect(const CollisionVolume&	   cv0, const CollisionVolume&				  cv1);
		GLACIER_API bool Intersect(const CollisionVolumeBSphere& a, const CollisionVolumeBSphere&			b);
		GLACIER_API bool Intersect(const CollisionVolumeAABB&	 a, const CollisionVolumeAABB&				b);
		GLACIER_API bool Intersect(const CollisionVolumeAABB&	 a, const CollisionVolumeBSphere&			b);
		GLACIER_API bool Intersect(const CollisionVolumeOBB&	 a, const CollisionVolumeBoundingBoxBase&	b);
		GLACIER_API bool Intersect(const CollisionVolumeOBB&	 a, const CollisionVolumeBSphere&			b);
		GLACIER_API bool Intersect(const glm::vec3& c1, const float& r1, const glm::vec3& c2, const float& r2);
		GLACIER_API bool LineIntersect(		  const glm::vec3& origin, const glm::vec3& dir, const CollisionVolume&		 cv);
		GLACIER_API bool LineBSphereIntersect(const glm::vec3& origin, const glm::vec3& dir, const CollisionVolumeBSphere& bsphere);
		GLACIER_API bool LineAABBIntersect(	  const glm::vec3& origin, const glm::vec3& dir, const CollisionVolumeAABB&	 aabb);
		GLACIER_API bool LineOBBIntersect(	  const glm::vec3& origin, const glm::vec3& dir, const CollisionVolumeOBB&	 obb);
		GLACIER_API void LineLineIntersection(const glm::vec3& p1, const glm::vec3& v1, const glm::vec3& p2, const glm::vec3& v2, glm::vec3& c1, glm::vec3& c2);
	};

	namespace VectMath
	{
		GLACIER_API void Calc2DBarycentric(const glm::vec3& pos, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, float& beta, float& gamma);

		GLACIER_API void WorldPosToScreenCoords(const glm::vec3& pos, const glm::mat4& view, const glm::mat4& proj, float& screen_x, float& screen_y);
		GLACIER_API void WorldPosToScreenCoords(const glm::vec3& pos, const glm::mat4& view_proj, float& screen_x, float& screen_y);
		GLACIER_API const glm::vec3 ScreenCoordsToWorldPos(const glm::vec3& screen_coords, const glm::mat4& view, const glm::mat4& proj);
		GLACIER_API const glm::vec3 ScreenCoordsToWorldPos(const glm::vec3& screen_coords, const glm::mat4& view_proj);

		GLACIER_API const glm::vec3 Lerp(const glm::vec3& a, const glm::vec3& b, const float& step);
		GLACIER_API const glm::vec3 Lerp2(const glm::vec3& a, const glm::vec3& b, const float& step);
	};

	namespace QuatMath
	{
		GLACIER_API const glm::quat Pow(const glm::quat& q, const float& x);
		GLACIER_API const glm::quat Slerp(const glm::quat& q0, const glm::quat& q1, const float& t);
		GLACIER_API const glm::quat Slerp2(const glm::quat& q0, const glm::quat& q1, const float& t);
	};

	namespace MatrixMath
	{
		GLACIER_API const glm::mat4 InterpolateTransform(const glm::mat4& m1, const glm::mat4& m2, const float& step);
		GLACIER_API const glm::mat4 InterpolateTransform2D(const glm::mat4& m1, const glm::mat4& m2, const glm::mat4& m3, const float weights[3]);
	}

	namespace Geometry2D
	{
		GLACIER_API const glm::vec3 ClosestPointOnSegment(const glm::vec3& pt, const glm::vec3& p0, const glm::vec3& p1);
		GLACIER_API const float ShortestDistanceToSegment(const glm::vec3& pt, const glm::vec3& p0, const glm::vec3& p1);
		GLACIER_API const float ShortestDistanceToSegmentSqr(const glm::vec3& pt, const glm::vec3& p0, const glm::vec3& p1);
		GLACIER_API const bool PointInTriangle(const glm::vec3& pt, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);
		GLACIER_API void BlendTriangle(const glm::vec3& pt, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, float weights[3]);
	}

	namespace Misc
	{
		/*
		* Runs a binary search on an array and returns the smaller index that the value falls in between.
		* 
		* Used in skeletal animation.
		*/
		template<class T>
		uint32_t BinarySearchApproximate(const std::vector<T>& v, const T& value)
		{
			uint32_t low = 0;
			uint32_t high = (const uint32_t)v.size() - 1;
			uint32_t mid = 0;
			while (low != high)
			{
				mid = (low + high) / 2;
				if (value < v[mid + 1])
				{
					if (value >= v[mid])
						return mid;
					else
						high = mid;
				}
				else
				{
					low = mid + 1;
				}
			}
			return mid;
		}
	}
};

#endif _GMathTools
