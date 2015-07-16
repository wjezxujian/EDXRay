#pragma once

#include "EDXPrerequisites.h"
#include "Math/Vector.h"
#include "../ForwardDecl.h"

namespace EDX
{
	namespace RayTracer
	{
		class Frame
		{
		public:
			Vector3 mX, mY, mZ;

		public:
			Frame()
			{
				mX = Vector3::UNIT_X;
				mY = Vector3::UNIT_Y;
				mZ = Vector3::UNIT_Z;
			};

			Frame(const Vector3& x,
				const Vector3& y,
				const Vector3& z
				)
				: mX(x)
				, mY(y)
				, mZ(z)
			{}

			__forceinline Frame(const Vector3& vNormal)
			{
				mZ = vNormal;
				Math::CoordinateSystem(mZ, &mX, &mY);
			}

			Vector3 LocalToWorld(const Vector3& vVec) const
			{
				return mX * vVec.x + mY * vVec.y + mZ * vVec.z;
			}

			Vector3 WorldToLocal(const Vector3& vVec) const
			{
				return Vector3(Math::Dot(vVec, mX), Math::Dot(vVec, mY), Math::Dot(vVec, mZ));
			}

			const Vector3& Binormal() const { return mX; }
			const Vector3& Tangent() const { return mY; }
			const Vector3& Normal() const { return mZ; }

		};

		class Intersection
		{
		public:
			uint	mPrimId, mTriId;
			float	mU, mV;
			float	mDist;

			Intersection()
				: mDist(float(Math::EDX_INFINITY))
				, mU(0.0f)
				, mV(0.0f)
			{
			}
		};

		class DifferentialGeom : public Intersection
		{
		public:
			Vector3 mPosition, mNormal;
			Vector2 mTexcoord;
			Vector3 mGeomNormal;
			Vector3 mDpdu, mDpdv;
			Vector3 mDndu, mDndv;

			// Differentials
			mutable Vector3 mDpdx, mDpdy;
			mutable float mDudx, mDudy, mDvdx, mDvdy;

			Frame mShadingFrame;

			const BSDF* mpBSDF;
			const AreaLight* mpAreaLight;

		public:
			DifferentialGeom()
				: mDudx(0.0f)
				, mDudy(0.0f)
				, mDvdx(0.0f)
				, mDvdy(0.0f)
			{
			}

			inline Vector3 WorldToLocal(const Vector3& vec) const
			{
				return mShadingFrame.WorldToLocal(vec);
			}
			inline Vector3 LocalToWorld(const Vector3& vec) const
			{
				return mShadingFrame.LocalToWorld(vec);
			}

			const Light* GetAreaLight() const
			{
				return (Light*)mpAreaLight;
			}
			Color Emit(const Vector3& dir) const; // Deprecated
			void ComputeDifferentials(const RayDifferential& ray) const;
		};

	}
}