#pragma once

namespace eae6320
{
	namespace Collision
	{
		struct sVector4
		{
			float x, y, z, w;

			sVector4(float x, float y, float z, float w);
		};

		class cMatrix4x4
		{
		private:
			float _matrix[4][4] = { 0.0 };

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

		};
	}
}