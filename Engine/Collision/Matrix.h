#pragma once
#include "Engine/Math/cQuaternion.h"

namespace eae6320
{
	namespace Collision
	{
		struct sVector4
		{
			float x, y, z, w;

			sVector4(float x, float y, float z, float w);
			sVector4();
			sVector4 operator*(const sVector4& other) const;
		};

		/**
		 * \brief
		 * Row vectors based matrix
		 */
		class cMatrix4x4
		{
		private:
			float _matrix[4][4] = {{0.0}};

		public:
			cMatrix4x4();
			cMatrix4x4(float matrix[4][4]);
			cMatrix4x4(const cMatrix4x4& other);
			sVector4 Row(int index) const;

			cMatrix4x4& operator=(const cMatrix4x4& other);
			cMatrix4x4 operator*(const cMatrix4x4& other) const;
			sVector4 operator*(const sVector4& other) const;

			static cMatrix4x4 CreateIdentity();
			static cMatrix4x4 CreateScale(float scaleX, float scaleY, float scaleZ);
			static cMatrix4x4 CreateTransform(float x, float y, float z);

			cMatrix4x4 Transpose();

			static cMatrix4x4 CreateXRotation(float radians);
			static cMatrix4x4 CreateYRotation(float radians);
			static cMatrix4x4 CreateZRotation(float radians);

		};
	}
}
