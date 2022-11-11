// Includes
//=========

#include "cQuaternion.h"

#include "sVector.h"

#include <cmath>
#include <Engine/Asserts/Asserts.h>

// Static Data
//============

namespace
{
	constexpr auto s_epsilon = 1.0e-9f;
}

// Interface
//==========

// Normalization
//--------------

void eae6320::Math::cQuaternion::Normalize()
{
	const auto length = std::sqrt( ( m_w * m_w ) + ( m_x * m_x ) + ( m_y * m_y ) + ( m_z * m_z ) );
	EAE6320_ASSERTF( length > s_epsilon, "Can't divide by zero" );
	const auto length_reciprocal = 1.0f / length;
	m_w *= length_reciprocal;
	m_x *= length_reciprocal;
	m_y *= length_reciprocal;
	m_z *= length_reciprocal;
}

eae6320::Math::cQuaternion eae6320::Math::cQuaternion::GetNormalized() const
{
	const auto length = std::sqrt( ( m_w * m_w ) + ( m_x * m_x ) + ( m_y * m_y ) + ( m_z * m_z ) );
	EAE6320_ASSERTF( length > s_epsilon, "Can't divide by zero" );
	const auto length_reciprocal = 1.0f / length;
	return cQuaternion( m_w * length_reciprocal, m_x * length_reciprocal, m_y * length_reciprocal, m_z * length_reciprocal );
}

eae6320::Math::sVector eae6320::Math::cQuaternion::GetEulerVector()
{
	// roll (x-axis rotation)
	const float sinr_cosp = 2 * (m_w * m_x + m_y * m_z);
	const float cosr_cosp = 1 - 2 * (m_x * m_x + m_y * m_y);
	const auto x = (float)std::atan2(sinr_cosp, cosr_cosp);

	// pitch (y-axis rotation)
	float sinp = 2 * (m_w * m_y - m_z * m_x);
	const auto y = (std::abs(sinp) >= 1) ? (float)std::copysign(M_PI / 2, sinp) : std::asin(sinp); // use 90 degrees if out of range

	// yaw (z-axis rotation)
	float siny_cosp = 2 * (m_w * m_z + m_x * m_y);
	float cosy_cosp = 1 - 2 * (m_y * m_y + m_z * m_z);
	const auto z = (float)std::atan2(siny_cosp, cosy_cosp);

	return sVector(x,y,z);
}

// Initialize / Clean Up
//----------------------

eae6320::Math::cQuaternion::cQuaternion( const float i_angleInRadians, const sVector i_axisOfRotation_normalized )
{
	const auto theta_half = i_angleInRadians * 0.5f;
	m_w = std::cos( theta_half );
	const auto sin_theta_half = std::sin( theta_half );
	m_x = i_axisOfRotation_normalized.x * sin_theta_half;
	m_y = i_axisOfRotation_normalized.y * sin_theta_half;
	m_z = i_axisOfRotation_normalized.z * sin_theta_half;
}
