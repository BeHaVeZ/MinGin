#pragma once
#include <Component.h>


namespace dae
{
	class Cube final : public Component
	{
	public:
		explicit Cube(const std::shared_ptr<GameObject>& gameObject,int colorIdx, int level, float cubeSpriteWidth, float cubeSpriteHeight);
		~Cube() = default;

		Cube(const Cube& other) = delete;
		Cube(Cube&& other) noexcept = delete;
		Cube& operator=(const Cube& other) = delete;
		Cube& operator=(Cube&& other) noexcept = delete;

		void TurnCube();
		void SlickSamTurnCube();
		bool GetIsTurned() const { return m_Turned; }


		void TurnCubeFirstColor() const;
		void TurnCubeSecondColor() const;
		void TurnCubeThirdColor() const;

		virtual void Update() override;
		virtual void Render() const override {};

	private:
		std::weak_ptr<GameObject> m_GameObject{};
		bool m_Turned,m_HalfTurned;
		int m_ColorIdx, m_Level;
		float m_CubeSpriteWidth, m_CubeSpriteHeight;
	};
}