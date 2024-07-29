#pragma once
#include <Component.h>


namespace dae
{
	class Cube final : public Component
	{
	public:
		explicit Cube(const std::shared_ptr<GameObject>& gameObject);
		~Cube() = default;

		Cube(const Cube& other) = delete;
		Cube(Cube&& other) noexcept = delete;
		Cube& operator=(const Cube& other) = delete;
		Cube& operator=(Cube&& other) noexcept = delete;

		void TurnCube();
		bool GetIsTurned() const { return m_Turned; }

		virtual void Update() override;
		virtual void Render() const override {};

	private:
		std::weak_ptr<GameObject> m_GameObject{};
		bool m_Turned;
	};
}