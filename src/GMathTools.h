#ifndef _GMathTools
#define _GMathTools

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
		inline bool NumInInterval(const float& num, const float& min, const float& max);
		bool PointInAABB(const glm::vec3& p, const glm::vec3& min, const glm::vec3& max);
		bool PointInOBB(const glm::vec3& p, const CollisionVolumeOBB& obb);
		inline bool IntervalIntersect(const float& min0, const float& max0, const float& min1, const float& max1);
		const float ClampNumToInterval(const float& num, const float& min, const float& max);
		const glm::vec3 ClampPointToInterval(const glm::vec3& c, const glm::vec3& min, const glm::vec3& max); 

		const float OBBMaxProjectionOnAxis(const CollisionVolumeBoundingBoxBase& obb, const glm::vec3& v);
		bool TestOBBsOnAxis(const CollisionVolumeBoundingBoxBase& a, const CollisionVolumeBoundingBoxBase& b, const glm::vec3& v);

		bool Intersect(const CollisionVolume&	   cv0, const CollisionVolume&				  cv1);
		bool Intersect(const CollisionVolumeBSphere& a, const CollisionVolumeBSphere&			b);
		bool Intersect(const CollisionVolumeAABB&	 a, const CollisionVolumeAABB&				b);
		bool Intersect(const CollisionVolumeAABB&	 a, const CollisionVolumeBSphere&			b);
		bool Intersect(const CollisionVolumeOBB&	 a, const CollisionVolumeBoundingBoxBase&	b);
		bool Intersect(const CollisionVolumeOBB&	 a, const CollisionVolumeBSphere&			b);
		bool Intersect(const glm::vec3& c1, const float& r1, const glm::vec3& c2, const float& r2);
		bool LineIntersect(		  const glm::vec3& origin, const glm::vec3& dir, const CollisionVolume&		 cv);
		bool LineBSphereIntersect(const glm::vec3& origin, const glm::vec3& dir, const CollisionVolumeBSphere& bsphere);
		bool LineAABBIntersect(	  const glm::vec3& origin, const glm::vec3& dir, const CollisionVolumeAABB&	 aabb);
		bool LineOBBIntersect(	  const glm::vec3& origin, const glm::vec3& dir, const CollisionVolumeOBB&	 obb);
		void LineLineIntersection(const glm::vec3& p1, const glm::vec3& v1, const glm::vec3& p2, const glm::vec3& v2, glm::vec3& c1, glm::vec3& c2);
	};

	namespace VectMath
	{
		void Calc2DBarycentric(const glm::vec3& pos, const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, float& beta, float& gamma);

		void WorldPosToScreenCoords(const glm::vec3& pos, const glm::mat4& view, const glm::mat4& proj, float& screen_x, float& screen_y);
		void WorldPosToScreenCoords(const glm::vec3& pos, const glm::mat4& view_proj, float& screen_x, float& screen_y);
		const glm::vec3 ScreenCoordsToWorldPos(const glm::vec3& screen_coords, const glm::mat4& view, const glm::mat4& proj);
		const glm::vec3 ScreenCoordsToWorldPos(const glm::vec3& screen_coords, const glm::mat4& view_proj);

		const glm::vec3 Lerp(const glm::vec3& a, const glm::vec3& b, const float& step);
		const glm::vec3 Lerp2(const glm::vec3& a, const glm::vec3& b, const float& step);
	};

	namespace QuatMath
	{
		const glm::quat Pow(const glm::quat& q, const float& x);
		const glm::quat Slerp(const glm::quat& q0, const glm::quat& q1, const float& t);
		const glm::quat Slerp2(const glm::quat& q0, const glm::quat& q1, const float& t);
	};

	namespace MatrixMath
	{
		const glm::mat4 InterpolateTransform(const glm::mat4& m1, const glm::mat4& m2, const float& step);
		const glm::mat4 InterpolateTransform2D(const glm::mat4& m1, const glm::mat4& m2, const glm::mat4& m3, const float weights[3]);
	}

	namespace Geometry2D
	{
		const glm::vec3 ClosestPointOnSegment(const glm::vec3& pt, const glm::vec3& p0, const glm::vec3& p1);
		const float ShortestDistanceToSegment(const glm::vec3& pt, const glm::vec3& p0, const glm::vec3& p1);
		const float ShortestDistanceToSegmentSqr(const glm::vec3& pt, const glm::vec3& p0, const glm::vec3& p1);
		const bool PointInTriangle(const glm::vec3& pt, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2);
		void BlendTriangle(const glm::vec3& pt, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, float weights[3]);
	}
};

#endif _GMathTools
