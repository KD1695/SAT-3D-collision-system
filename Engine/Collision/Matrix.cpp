#include <Engine/Collision/Matrix.h>
#include <corecrt_math.h>

/// <summary>
/// Create empty 4x4 matrix
/// </summary>
eae6320::Collision::cMatrix4x4::cMatrix4x4()
{

}

/// <summary>
/// Create 4x4 matrix from 2d float array
/// </summary>
/// <param name="matrix">2d 4x4 float array</param>
eae6320::Collision::cMatrix4x4::cMatrix4x4(float matrix[4][4])
{
	for (int i = 0; i < 4; i++)
	{
		_matrix[i][0] = matrix[i][0];
		_matrix[i][1] = matrix[i][1];
		_matrix[i][2] = matrix[i][2];
		_matrix[i][3] = matrix[i][3];
	}
}

/// <summary>
/// copy constructor
/// </summary>
/// <param name="other">other matrix</param>
eae6320::Collision::cMatrix4x4::cMatrix4x4(const cMatrix4x4& other)
{
	for (int i = 0; i < 4; i++)
	{
		_matrix[i][0] = other.Row(i).x;
		_matrix[i][1] = other.Row(i).y;
		_matrix[i][2] = other.Row(i).z;
		_matrix[i][3] = other.Row(i).w;
	}
}

/// <summary>
/// Get row at specified index from matrix
/// </summary>
/// <param name="index">int index of row</param>
/// <returns>sVector4 row based vector</returns>
eae6320::Collision::sVector4 eae6320::Collision::cMatrix4x4::Row(int index) const
{
	return sVector4(_matrix[index][0], _matrix[index][1], _matrix[index][2], _matrix[index][3]);
}

eae6320::Collision::cMatrix4x4& eae6320::Collision::cMatrix4x4::operator=(const cMatrix4x4& other)
{
	for (int i = 0; i < 4; i++)
	{
		_matrix[i][0] = other.Row(i).x;
		_matrix[i][1] = other.Row(i).y;
		_matrix[i][2] = other.Row(i).z;
		_matrix[i][3] = other.Row(i).w;
	}
	return *this;
}

/// <summary>
/// 4x4 Matrix multiplication
/// </summary>
/// <param name="other">other matrix</param>
/// <returns>cMatrix4x4 matrix after multiplication</returns>
eae6320::Collision::cMatrix4x4 eae6320::Collision::cMatrix4x4::operator*(const cMatrix4x4& other) const
{
	float arr[4][4] = {{0}};
	for (int i = 0; i < 4; i++)
	{
		arr[i][0] = _matrix[i][0] * other.Row(0).x + _matrix[i][1] * other.Row(1).x + _matrix[i][2] * other.Row(2).x + _matrix[i][3] * other.Row(3).x;
		arr[i][1] = _matrix[i][0] * other.Row(0).y + _matrix[i][1] * other.Row(1).y + _matrix[i][2] * other.Row(2).y + _matrix[i][3] * other.Row(3).y;
		arr[i][2] = _matrix[i][0] * other.Row(0).z + _matrix[i][1] * other.Row(1).z + _matrix[i][2] * other.Row(2).z + _matrix[i][3] * other.Row(3).z;
		arr[i][3] = _matrix[i][0] * other.Row(0).w + _matrix[i][1] * other.Row(1).w + _matrix[i][2] * other.Row(2).w + _matrix[i][3] * other.Row(3).w;
	}
	return cMatrix4x4(arr);
}

/// <summary>
/// Opertaor overloading for vector cross product
/// </summary>
/// <param name="other">other vector</param>
/// <returns>sVector4 cross product result</returns>
eae6320::Collision::sVector4 eae6320::Collision::sVector4::operator*(const sVector4& other) const
{
	return sVector4(y*other.z-z*other.y, -(x*other.z-z*other.x), x*other.y-y*other.x, 1);
}

/// <summary>
/// Matrix multiplication between 1x4 and 4x4 matrix. 
/// NOTE: the sVector4 operand appears on RHS of operator and cMatrix4x4 on the LHS
/// </summary>
/// <param name="other">sVector4 type row vector</param>
/// <returns>sVector4 row vector as [1x4] * [4x4] = [1x4]</returns>
eae6320::Collision::sVector4 eae6320::Collision::cMatrix4x4::operator*(const sVector4& other) const
{
	float arr[4] = { 0 };
	for (int i = 0; i < 4; i++)
	{
		arr[i] = other.x * _matrix[0][i] + other.y * _matrix[1][i] + other.z * _matrix[2][i] + other.w * _matrix[3][i];
	}
	return sVector4(arr[0], arr[1], arr[2], arr[3]);
}

/// <summary>
/// create indentity matrix
/// </summary>
/// <returns></returns>
eae6320::Collision::cMatrix4x4 eae6320::Collision::cMatrix4x4::CreateIdentity()
{
	float arr[4][4] = {{0}};
	for (int i = 0; i < 4; i++)
		arr[i][i] = 1;
	return cMatrix4x4(arr);
}

/// <summary>
/// Create scale matrix based on x,y,z scale
/// </summary>
/// <param name="scaleX"></param>
/// <param name="scaleY"></param>
/// <param name="scaleZ"></param>
/// <returns></returns>
eae6320::Collision::cMatrix4x4 eae6320::Collision::cMatrix4x4::CreateScale(float scaleX, float scaleY, float scaleZ)
{
	float arr[4][4] = {{0}};
	arr[0][0] = scaleX;
	arr[1][1] = scaleY;
	arr[2][2] = scaleZ;
	arr[3][3] = 1;

	return cMatrix4x4(arr);
}

/// <summary>
/// Create Transform matrix based on x,y,z position
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="z"></param>
/// <returns></returns>
eae6320::Collision::cMatrix4x4 eae6320::Collision::cMatrix4x4::CreateTransform(float x, float y, float z)
{
	float arr[4][4] = {{0}};
	for (int i = 0; i < 4; i++)
		arr[i][i] = 1;
	arr[3][0] = x;
	arr[3][1] = y;
	arr[3][2] = z;
	return cMatrix4x4(arr);
}

/// <summary>
/// Create transpose of a matrix
/// </summary>
/// <returns></returns>
eae6320::Collision::cMatrix4x4 eae6320::Collision::cMatrix4x4::Transpose()
{
	float arr[4][4];
	for (int i = 0; i < 4; i++)
	{
		arr[i][0] = _matrix[0][i];
		arr[i][1] = _matrix[1][i];
		arr[i][2] = _matrix[2][i];
		arr[i][3] = _matrix[3][i];
	}
	return cMatrix4x4(arr);
}

/// <summary>
/// Create X rotation matrix
/// </summary>
/// <param name="radians">rotation in x-axis, euler angle in radians</param>
/// <returns>4x4 rotation matrix</returns>
eae6320::Collision::cMatrix4x4 eae6320::Collision::cMatrix4x4::CreateXRotation(float radians)
{
	const float sin = sinf(radians);
	const float cos = cosf(radians);
	float arr[4][4] = {
		{1, 0, 0, 0},
		{0, cos, sin, 0},
		{0, -sin, cos, 0},
		{0, 0, 0, 1}};
	
	return cMatrix4x4(arr);
}

/// <summary>
/// Create Y rotation matrix
/// </summary>
/// <param name="radians">rotation in y-axis, euler angle in radians</param>
/// <returns>4x4 rotation matrix</returns>
eae6320::Collision::cMatrix4x4 eae6320::Collision::cMatrix4x4::CreateYRotation(float radians)
{
	const float sin = sinf(radians);
	const float cos = cosf(radians);
	float arr[4][4] = {
		{cos, 0, -sin, 0},
		{0, 1, 0, 0},
		{sin, 0, cos, 0},
		{0, 0, 0, 1}};
	
	return cMatrix4x4(arr);
}

/// <summary>
/// Create Z rotation matrix
/// </summary>
/// <param name="radians">rotation in z-axis, euler angle in radians</param>
/// <returns>4x4 rotation matrix</returns>
eae6320::Collision::cMatrix4x4 eae6320::Collision::cMatrix4x4::CreateZRotation(float radians)
{
	const float sin = sinf(radians);
	const float cos = cosf(radians);
	float arr[4][4] = {
		{cos, sin, 0, 0},
		{-sin, cos, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 0, 1}};
	
	return cMatrix4x4(arr);
}

eae6320::Collision::sVector4::sVector4(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

eae6320::Collision::sVector4::sVector4()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}
