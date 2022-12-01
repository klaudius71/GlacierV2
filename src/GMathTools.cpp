#include "gpch.h"
#include "GMathTools.h"
#include "Glacier.h"
//#include "CollisionVolumeBSphere.h"
//#include "CollisionVolumeAABB.h"
//#include "CollisionVolumeBoundingBoxBase.h"
//#include "CollisionVolumeOBB.h"
//
//#include "delaunator.hpp"

namespace GMathTools
{
	namespace Collision
	{
		inline bool NumInInterval(const float& num, const float& min, const float& max)
		{
			return num > min && num < max;
		}
		bool PointInAABB(const glm::vec3& p, const glm::vec3& min, const glm::vec3& max)
		{
			return NumInInterval(p.x, min.x, max.x)
				&& NumInInterval(p.y, min.y, max.y)
				&& NumInInterval(p.z, min.z, max.z);
		}
		//bool PointInOBB(const glm::vec3& p, const CollisionVolumeOBB& obb)
		//{
		//	glm::vec3 inv_p(p * obb.getCurrentWorldInv());
		//	const glm::vec3& min = obb.getModel()->getMinAABB(), & max = obb.getModel()->getMaxAABB();
		//	return NumInInterval(p.X(), min.X(), max.X())
		//		&& NumInInterval(p.Y(), min.Y(), max.Y())
		//		&& NumInInterval(p.Z(), min.Z(), max.Z());
		//}
		inline bool IntervalIntersect(const float& min0, const float& max0, const float& min1, const float& max1)
		{
			return (min1 < max0) && (min0 < max1);
		}
		const float ClampNumToInterval(const float& num, const float& min, const float& max)
		{
			if (num < min) return min;
			else if (num > max) return max;
			else return num;
		}
		const glm::vec3 ClampPointToInterval(const glm::vec3& c, const glm::vec3& min, const glm::vec3& max)
		{
			return glm::vec3(ClampNumToInterval(c.x, min.x, max.x),
						ClampNumToInterval(c.y, min.y, max.y),
						ClampNumToInterval(c.z, min.z, max.z));
		}

		//const float OBBMaxProjectionOnAxis(const CollisionVolumeBoundingBoxBase& obb, const glm::vec3& v)
		//{
		//	const glm::vec3 vPrime = v * obb.getCurrentWorldInv();
		//	const float ProjMax = (abs(vPrime.X() * obb.getHalfDiag().X()) + 
		//						   abs(vPrime.Y() * obb.getHalfDiag().Y()) + 
		//						   abs(vPrime.Z() * obb.getHalfDiag().Z()))/
		//						   v.mag();
		//	return obb.getScaleSqr() * ProjMax;
		//}
		//bool TestOBBsOnAxis(const CollisionVolumeBoundingBoxBase& a, const CollisionVolumeBoundingBoxBase& b, const glm::vec3& v)
		//{ 
		//	if (v.magSqr() > FLT_EPSILON)
		//	{
		//		float D = abs((b.getCenter() - a.getCenter()).dot(v)) / v.mag();
		//		float PA = OBBMaxProjectionOnAxis(a, v);
		//		float PB = OBBMaxProjectionOnAxis(b, v);
		//		return D <= PA + PB;
		//	}
		//	else return true;
		//}

		//bool Intersect(const CollisionVolume& cv0, const CollisionVolume& cv1)
		//{
		//	return cv0.IntersectAccept(cv1);
		//}

		bool Intersect(const glm::vec3& c1, const float& r1, const glm::vec3& c2, const float& r2)
		{
			return glm::length(c1 - c2) < (r1 + r2);
		}

		//bool Intersect(const CollisionVolumeBSphere& a, const CollisionVolumeBSphere& b)
		//{
		//	return (a.getBSphereCenter() - b.getBSphereCenter()).mag() < (a.getRadius() + b.getRadius());
		//}

		//bool Intersect(const CollisionVolumeAABB& a, const CollisionVolumeBSphere& b)
		//{
		//	return (b.getBSphereCenter() - ClampPointToInterval(b.getBSphereCenter(), a.getMin(), a.getMax())).mag() < b.getRadius();
		//}
		//bool Intersect(const CollisionVolumeAABB& a, const CollisionVolumeAABB& b)
		//{
		//	const glm::vec3& vol1Min = a.getMin(), & vol1Max = a.getMax();
		//	const glm::vec3& vol2Min = b.getMin(), & vol2Max = b.getMax();
		//	return IntervalIntersect(vol1Min.X(), vol1Max.X(), vol2Min.X(), vol2Max.X())
		//		&& IntervalIntersect(vol1Min.Y(), vol1Max.Y(), vol2Min.Y(), vol2Max.Y())
		//		&& IntervalIntersect(vol1Min.Z(), vol1Max.Z(), vol2Min.Z(), vol2Max.Z());
		//}

		//bool Intersect(const CollisionVolumeOBB& a, const CollisionVolumeBoundingBoxBase& b)
		//{
		//	const glm::mat4& aWorld = a.getCurrentWorld();
		//	const glm::mat4& bWorld = b.getCurrentWorld();
		//
		//	glm::vec3 anorm1 = aWorld.get(ROW_0);
		//	anorm1.W() = 0.0f; // -_-
		//	if (!TestOBBsOnAxis(a, b, anorm1)) return false;
		//	glm::vec3 anorm2 = aWorld.get(ROW_1);
		//	anorm2.W() = 0.0f; // -_-
		//	if (!TestOBBsOnAxis(a, b, anorm2)) return false;
		//	glm::vec3 anorm3 = aWorld.get(ROW_2);
		//	anorm3.W() = 0.0f; // -_-
		//	if (!TestOBBsOnAxis(a, b, anorm3)) return false;
		//	glm::vec3 bnorm1 = bWorld.get(ROW_0);
		//	bnorm1.W() = 0.0f; // -_-
		//	if (!TestOBBsOnAxis(a, b, bnorm1)) return false;
		//	glm::vec3 bnorm2 = bWorld.get(ROW_1);
		//	bnorm2.W() = 0.0f; // -_-
		//	if (!TestOBBsOnAxis(a, b, bnorm2)) return false;
		//	glm::vec3 bnorm3 = bWorld.get(ROW_2);
		//	bnorm3.W() = 0.0f; // -_-
		//	if (!TestOBBsOnAxis(a, b, bnorm3)) return false;
		//	glm::vec3 a1xb1 = anorm1.cross(bnorm1);
		//	a1xb1.W() = 0.0f; // -_-
		//	if (!TestOBBsOnAxis(a, b, a1xb1)) return false;
		//	glm::vec3 a1xb2 = anorm1.cross(bnorm2);
		//	a1xb2.W() = 0.0f; // -_-
		//	if (!TestOBBsOnAxis(a, b, a1xb2)) return false;
		//	glm::vec3 a1xb3 = anorm1.cross(bnorm3);
		//	a1xb3.W() = 0.0f; // -_-
		//	if (!TestOBBsOnAxis(a, b, a1xb3)) return false;
		//	glm::vec3 a2xb1 = anorm2.cross(bnorm1);
		//	a2xb1.W() = 0.0f; // -_-
		//	if (!TestOBBsOnAxis(a, b, a2xb1)) return false;
		//	glm::vec3 a2xb2 = anorm2.cross(bnorm2);
		//	a2xb2.W() = 0.0f; // -_-
		//	if (!TestOBBsOnAxis(a, b, a2xb2)) return false;
		//	glm::vec3 a2xb3 = anorm2.cross(bnorm3);
		//	a2xb3.W() = 0.0f; // -_-
		//	if (!TestOBBsOnAxis(a, b, a2xb3)) return false;
		//	glm::vec3 a3xb1 = anorm3.cross(bnorm1);
		//	a3xb1.W() = 0.0f; // -_-
		//	if (!TestOBBsOnAxis(a, b, a3xb1)) return false;
		//	glm::vec3 a3xb2 = anorm3.cross(bnorm2);
		//	a3xb2.W() = 0.0f; // -_-
		//	if (!TestOBBsOnAxis(a, b, a3xb2)) return false;
		//	glm::vec3 a3xb3 = anorm3.cross(bnorm3);
		//	a3xb3.W() = 0.0f; // -_-
		//	if (!TestOBBsOnAxis(a, b, a3xb2)) return false;
		//
		//	return true;
		//}
		//bool Intersect(const CollisionVolumeOBB& a, const CollisionVolumeBSphere& b)
		//{
		//	glm::vec3 clmp_pt(b.getCenter() * a.getCurrentWorldInv());
		//	clmp_pt = ClampPointToInterval(clmp_pt, a.getModel()->getMinAABB(), a.getModel()->getMaxAABB());
		//	clmp_pt *= a.getCurrentWorld();
		//	return (b.getCenter() - clmp_pt).mag() < b.getRadius();
		//}

		//bool LineIntersect(const glm::vec3& origin, const glm::vec3& dir, const CollisionVolume& cv)
		//{
		//	return cv.IntersectVisit(origin, dir);
		//}
		//bool LineBSphereIntersect(const glm::vec3& origin, const glm::vec3& dir, const CollisionVolumeBSphere& bsphere)
		//{
		//	float t = (dir.dot(bsphere.getCenter() - origin) / dir.dot(dir));
		//	if (t < 0) t = 0;
		//	glm::vec3 Q = origin + t * dir;
		//	return (bsphere.getCenter() - Q).magSqr() <= (bsphere.getRadius() * bsphere.getRadius());
		//}
		inline void FindIntervalOnLine(const float& p, const float& v, const float& min, const float& max, float& s, float& t)
		{
			if (v == 0)
			{
				//if (p < min || p > max) return;
				//else
				//{
					s = FLT_MIN;
					t = FLT_MAX;
				//}
			}
			else
			{
				float a = (min - p) / v;
				float b = (max - p) / v;
				s = a < b ? a : b;
				t = a > b ? a : b;
			}
		}
		//bool LineAABBIntersect(const glm::vec3& origin, const glm::vec3& dir, const CollisionVolumeAABB& aabb)
		//{
		//	float sx, tx, sy, ty, sz, tz;
		//	FindIntervalOnLine(origin.X(), dir.X(), aabb.getMin().X(), aabb.getMax().X(), sx, tx);
		//	FindIntervalOnLine(origin.Y(), dir.Y(), aabb.getMin().Y(), aabb.getMax().Y(), sy, ty);
		//	FindIntervalOnLine(origin.Z(), dir.Z(), aabb.getMin().Z(), aabb.getMax().Z(), sz, tz);
		//	return IntervalIntersect(sx, tx, sy, ty)
		//		&& IntervalIntersect(sy, ty, sz, tz)
		//		&& IntervalIntersect(sz, tz, sx, tx);
		//}
		//bool LineOBBIntersect(const glm::vec3& origin, const glm::vec3& dir, const CollisionVolumeOBB& obb)
		//{
		//	const glm::vec3 originPrime(origin * obb.getCurrentWorldInv());
		//	glm::vec3 dirPrime(dir);
		//	dirPrime.W() = 0.0f; // -_-
		//	dirPrime *= obb.getCurrentWorldInv();
		//	float sx, tx, sy, ty, sz, tz;
		//	FindIntervalOnLine(originPrime.X(), dirPrime.X(), obb.getModel()->getMinAABB().X(), obb.getModel()->getMaxAABB().X(), sx, tx);
		//	FindIntervalOnLine(originPrime.Y(), dirPrime.Y(), obb.getModel()->getMinAABB().Y(), obb.getModel()->getMaxAABB().Y(), sy, ty);
		//	FindIntervalOnLine(originPrime.Z(), dirPrime.Z(), obb.getModel()->getMinAABB().Z(), obb.getModel()->getMaxAABB().Z(), sz, tz);
		//	return IntervalIntersect(sx, tx, sy, ty)
		//		&& IntervalIntersect(sy, ty, sz, tz)
		//		&& IntervalIntersect(sz, tz, sx, tx);
		//}
		//void LineLineIntersection(const glm::vec3& p1, const glm::vec3& v1, const glm::vec3& p2, const glm::vec3& v2, glm::vec3& c1, glm::vec3& c2)
		//{
		//	const float a = v1.dot(v1);
		//	const float b = v1.dot(v2);
		//	const float c = v2.dot(v2);
		//	const float det = b * b - a * c;
		//	const glm::vec3 Pv = p1 - p2;
		//	const float d = Pv.dot(v1);
		//	const float e = Pv.dot(v2);
		//	if (det == 0.0f)
		//	{
		//		c1 = p1;
		//		c2 = p2 + (e / c) * v2;
		//	}
		//	else
		//	{
		//		c1 = p1 + ((c * d - b * e) / det) * v1;
		//		c2 = p2 + ((d * b - a * e) / det) * v2;
		//	}
		//}
	}

	namespace VectMath
	{
		void Calc2DBarycentric(const glm::vec3& pos, const glm::vec3& point_a, const glm::vec3& point_b, const glm::vec3& point_c, float& beta, float& gamma)
		{
			const glm::vec3 v0 = point_b - point_a;
			const glm::vec3 v1 = point_c - point_b;
			const glm::vec3 v2 = pos - point_a;
			const float a = glm::dot(v0, v0);
			const float b = glm::dot(v1, v0);
			const float c = glm::dot(v1, v1);
			const float d = glm::dot(v2, v0);
			const float e = glm::dot(v2, v1);
			const float den = a * c - b * b;
			if (den == 0.0f)
			{
				beta = 0.0f;
				gamma = 0.0f;
				return;
			}
			beta = (d * c - b * e) / den;
			gamma = (a * e - d * b) / den;
		}
			 
		//void WorldPosToScreenCoords(const glm::vec3& pos, const glm::mat4& view, const glm::mat4& proj, float& screen_x, float& screen_y)
		//{
		//	glm::vec3 ndc = pos * view * proj;
		//	const float inv_w = 1.0f / ndc.W();
		//	ndc *= inv_w;
		//	if (ndc.Z() > 1.0f)
		//	{
		//		ndc.X() = ndc.X() < 0.0f ? 1.1f : -1.1f;
		//		ndc.Y() = 0.0f;
		//	}
		//	screen_x = ((ndc.X() + 1.0f) / 2.0f) * Glacier::GetWindowWidth();
		//	screen_y = Glacier::GetWindowHeight() - ((ndc.Y() + 1.0f) / 2.0f) * Glacier::GetWindowHeight();
		//}
		//void WorldPosToScreenCoords(const glm::vec3& pos, const glm::mat4& view_proj, float& screen_x, float& screen_y)
		//{
		//	glm::vec3 ndc = pos * view_proj;
		//	const float inv_w = 1.0f / ndc.W();
		//	ndc *= inv_w;
		//	if (ndc.Z() > 1.0f)
		//	{
		//		ndc.X() = ndc.X() < 0.0f ? 1.1f : -1.1f;
		//		ndc.Y() = 0.0f;
		//	}
		//	screen_x = ((ndc.X() + 1.0f) / 2.0f) * Glacier::GetWindowWidth();
		//	screen_y = Glacier::GetWindowHeight() - ((ndc.Y() + 1.0f) / 2.0f) * Glacier::GetWindowHeight();
		//}
		//const glm::vec3 ScreenCoordsToWorldPos(const glm::vec3& screen_coords, const glm::mat4& view, const glm::mat4& proj)
		//{
		//	const glm::mat4 invProjectView((view * proj).getInv());
		//	glm::vec3 screenPos((screen_coords.X() / Glacier::GetWindowWidth() - 0.5f) * 2.0f, (screen_coords.Y() / Glacier::GetWindowHeight() - 0.5f) * 2.0f, screen_coords.Z(), screen_coords.W());
		//	screenPos *= invProjectView;
		//	return screenPos * (1.0f / screenPos.W());
		//}
		//const glm::vec3 ScreenCoordsToWorldPos(const glm::vec3& screen_coords, const glm::mat4& view_proj)
		//{
		//	const glm::mat4 invProjectView(view_proj.getInv());
		//	glm::vec3 screenPos((screen_coords.X() / Glacier::GetWindowWidth() - 0.5f) * 2.0f, (screen_coords.Y() / Glacier::GetWindowHeight() - 0.5f) * 2.0f, screen_coords.Z(), screen_coords.W());
		//	screenPos *= invProjectView;
		//	return screenPos * (1.0f / screenPos.W());
		//}

		const glm::vec3 Lerp(const glm::vec3& a, const glm::vec3& b, const float& step)
		{
			return a + ((b-a) * step);
		}
		const glm::vec3 Lerp2(const glm::vec3& a, const glm::vec3& b, const float& step)
		{
			return a * (1.0f - step) + b * step;
		}
	}

	namespace QuatMath
	{
		const glm::quat Pow(const glm::quat& q, const float& pow) // glm
		{
			if (pow < FLT_EPSILON && pow > -FLT_EPSILON) 
				return glm::quat(0, 0, 0, 1);

			float& x = *(((float*)&q) + 0);
			float& y = *(((float*)&q) + 1);
			float& z = *(((float*)&q) + 2);
			float& w = *(((float*)&q) + 3);
	
			const float q_mag = glm::length(q);
			float angle;
			if (fabsf(w / q_mag) > cosf(.5f))
			{
				const float q_vec_mag_sqr = x * x + y * y + z * z;

				if (q_vec_mag_sqr < FLT_MIN) 
					return glm::quat(0, 0, 0, powf(w, pow));

				angle = asinf(q_vec_mag_sqr / q_mag);
			}
			else
			{
				angle = acosf(w / q_mag);
			}

			const float new_angle = angle * pow;
			const float div = sinf(new_angle) / sinf(angle);
			const float mag = powf(q_mag, pow - 1);
			return glm::quat(x * div * mag, y * div * mag, z * div * mag, cosf(new_angle) * q_mag * mag);
		}
		const glm::quat Slerp(const glm::quat& q0, const glm::quat& q1, const float& t)
		{
			return q0 * Pow((glm::inverse(q0) * q1), t);
		}
		const glm::quat Slerp2(const glm::quat& q0, const glm::quat& q1, const float& t)
		{
			glm::quat q2(q1);

			float& x0 = *(((float*)&q0) + 0);
			float& y0 = *(((float*)&q0) + 1);
			float& z0 = *(((float*)&q0) + 2);
			float& w0 = *(((float*)&q0) + 3);

			float& x2 = *(((float*)&q2) + 0);
			float& y2 = *(((float*)&q2) + 1);
			float& z2 = *(((float*)&q2) + 2);
			float& w2 = *(((float*)&q2) + 3);

			float cosTheta = glm::dot(q0, q1);
			if (cosTheta < 0.0f)
			{
				q2 = -q1;
				cosTheta = -cosTheta;
			}

			if (cosTheta > (1.0f - FLT_EPSILON))
			{
				x2 = x0 + (x2 - x0) * t;
				y2 = y0 + (y2 - y0) * t;
				z2 = z0 + (z2 - z0) * t;
				w2 = w0 + (w2 - w0) * t;
				return q2;
			}
			else
			{
				const float angle = acosf(cosTheta);
				return (sinf((1.0f - t) * angle) * q0 + sinf(t * angle) * q2) / sinf(angle);
			}
		}
	}

	namespace MatrixMath
	{
		const glm::mat4 InterpolateTransform(const glm::mat4& m1, const glm::mat4& m2, const float& step)
		{
			const glm::vec4& trans1 = m1[3];
			const glm::quat& rot1 = glm::quat(m1);
			const glm::vec3 scl1 = glm::vec3(glm::length(m1[0]), glm::length(m1[1]), glm::length(m1[2]));

			const glm::vec4& trans2 = m2[3];
			const glm::quat& rot2 = glm::quat(m2);
			const glm::vec3& scl2 = glm::vec3(glm::length(m2[0]), glm::length(m2[1]), glm::length(m2[2]));

			return glm::translate(glm::mat4(1.0f), VectMath::Lerp2(trans1, trans2, step)) * glm::mat4_cast(glm::slerp(rot1, rot2, step)) * glm::scale(glm::mat4(1.0f), VectMath::Lerp2(scl1, scl2, step));
		}
		const glm::mat4 InterpolateTransform2D(const glm::mat4& m1, const glm::mat4& m2, const glm::mat4& m3, const float weights[3])
		{
			const float first_second_weight_ratio = weights[1] / (weights[0] + weights[1]);
			if (weights[0] + weights[1] == 0.0f)
				return m3;

			const glm::vec4& trans1 = m1[3];
			const glm::quat& rot1 = glm::quat(m1);
			const glm::vec3 scl1 = glm::vec3(glm::length(m1[0]), glm::length(m1[1]), glm::length(m1[2]));

			const glm::vec4& trans2 = m2[3];
			const glm::quat& rot2 = glm::quat(m2);
			const glm::vec3 scl2 = glm::vec3(glm::length(m2[0]), glm::length(m2[1]), glm::length(m2[2]));

			const glm::vec4& trans3 = m3[3];
			const glm::quat& rot3 = glm::quat(m3);
			const glm::vec3 scl3 = glm::vec3(glm::length(m3[0]), glm::length(m3[1]), glm::length(m3[2]));

			const glm::vec3& trans_lerp = VectMath::Lerp2(VectMath::Lerp2(trans1, trans2, first_second_weight_ratio), trans3, weights[2]);
			const glm::quat& rot_slerp = QuatMath::Slerp2(QuatMath::Slerp2(rot1, rot2, first_second_weight_ratio), rot3, weights[2]);
			const glm::vec3& scl_lerp = VectMath::Lerp2(VectMath::Lerp2(scl1, scl2, first_second_weight_ratio), scl3, weights[2]);

			return glm::translate(glm::mat4(1.0f), trans_lerp) * glm::mat4(rot_slerp) * glm::scale(glm::mat4(1.0f), scl_lerp);
		}
	}

	namespace Geometry2D
	{
		const glm::vec3 ClosestPointOnSegment(const glm::vec3& pt, const glm::vec3& p0, const glm::vec3& p1)
		{
			const glm::vec3 dir = p1 - p0;
			const glm::vec3 a = pt - p0;
			const float t = glm::dot(a, dir) / glm::dot(dir, dir);

			if (t <= 0.0f)
				return p0;
			else if (t >= 1.0f)
				return p1;
			else
				return p0 + t * dir;
		}
		const float ShortestDistanceToSegment(const glm::vec3& pt, const glm::vec3& p0, const glm::vec3& p1)
		{
			const glm::vec3 dir = p1 - p0;
			const glm::vec3 a = pt - p0;
			const float t = glm::dot(a, dir) / glm::dot(dir, dir);

			if (t <= 0.0f)
				return glm::length(a);
			else if (t >= 1.0f)
				return glm::length(pt - (p0 + t * dir));
			else
				return glm::length(pt - p1);
		}
		const float ShortestDistanceToSegmentSqr(const glm::vec3& pt, const glm::vec3& p0, const glm::vec3& p1)
		{
			const glm::vec3 dir = p1 - p0;
			const glm::vec3 a = pt - p0;
			const float t = glm::dot(a, dir) / glm::dot(dir, dir);

			if (t <= 0.0f)
				return glm::length2(a);
			else if (t >= 1.0f)
				return glm::length2(pt - (p0 + t * dir));
			else
				return glm::length2(pt - p1);
		}
		const bool PointInTriangle(const glm::vec3& pt, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2)
		{
			//const glm::vec3 a = p0 - pt;
			//const glm::vec3 b = p1 - pt;
			//const glm::vec3 c = p2 - pt;
			//
			//const float& acrossb = a.X() * b.Y() - a.Y() * b.X();
			//const float& bcrossc = b.X() * c.Y() - b.Y() * c.X();
			//
			//bool orient = acrossb > 0.0f;
			//
			//if ((bcrossc > 0.0f) != orient)
			//	return false;
			//
			//const float& ccrossa = c.X() * a.Y() - c.Y() - a.X();
			//return (ccrossa > 0.0f) == orient;

			const glm::vec3 v0 = p1 - p0;
			const glm::vec3 v1 = p2 - p0;
			const glm::vec3 v2 = pt - p0;

			const float a = glm::dot(v0, v0);
			const float b = glm::dot(v0, v1);
			const float c = glm::dot(v1, v1);
			const float d = glm::dot(v2, v0);
			const float e = glm::dot(v2, v1);
			const float denom = (a * c - b * b);
			if (denom == 0.0f)
				return false;

			const float u = (c * d - b * e) / denom;
			const float v = (a * e - b * d) / denom;
			return u >= 0.0f && v >= 0.0f && (u + v) < 1.0f;
		}
		void BlendTriangle(const glm::vec3& pt, const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2, float weights[3])
		{
			if (pt == p0)
			{
				weights[0] = 1.0f;
				weights[1] = 0.0f;
				weights[2] = 0.0f;
				return;
			}
			else if (pt == p1)
			{
				weights[0] = 0.0f;
				weights[1] = 1.0f;
				weights[2] = 0.0f;
				return;
			}
			else if (pt == p2)
			{
				weights[0] = 0.0f;
				weights[1] = 0.0f;
				weights[2] = 1.0f;
				return;
			}

			const glm::vec3 v0 = p1 - p0;
			const glm::vec3 v1 = p2 - p0;
			const glm::vec3 v2 = pt - p0;

			const float a = glm::dot(v0, v0);
			const float b = glm::dot(v0, v1);
			const float c = glm::dot(v1, v1);
			const float d = glm::dot(v2, v0);
			const float e = glm::dot(v2, v1);
			const float denom = (a * c - b * b);
			if (denom == 0.0f)
			{
				weights[0] = 1.0f;
				weights[1] = 0.0f;
				weights[2] = 0.0f;
				return;
			}
	
			weights[1] = (c * d - b * e) / denom;
			weights[2] = (a * e - b * d) / denom;
			weights[0] = 1.0f - (weights[1] + weights[2]);
		}
	}
}
