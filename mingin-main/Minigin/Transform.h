#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		Transform();
		~Transform() = default;
		Transform(const Transform& other) = default;
		Transform& operator=(const Transform& other) = default;
		Transform(Transform&& other) noexcept = default;
		Transform& operator=(Transform&& other) noexcept = default;



		const glm::vec3& GetPosition() const { return m_position; }
		const glm::vec3& GetRotation() const { return m_rotation; }
		const glm::vec3& GetScale() const { return m_scale; }
		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z);
		void SetScale(float x, float y, float z);

	private:
		glm::vec3 m_position;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;
	};
}
