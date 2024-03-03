#include "pch.h"

namespace dae {

	Transform::Transform()
		: m_position(0.0f, 0.0f, 0.0f), m_rotation(0.0f, 0.0f, 0.0f), m_scale(1.0f, 1.0f, 1.0f)
	{
	}

	void Transform::SetPosition(const float x, const float y, const float z)
	{
		m_position.x = x;
		m_position.y = y;
		m_position.z = z;
	}
	void Transform::SetRotation(const float x, const float y, const float z)
	{
		m_rotation.x = x;
		m_rotation.y = y;
		m_rotation.z = z;
	}

	void Transform::SetScale(const float x, const float y, const float z)
	{
		m_scale.x = x;
		m_scale.y = y;
		m_scale.z = z;
	}
}