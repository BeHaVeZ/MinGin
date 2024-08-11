#pragma once
#include <memory>
#include "Component.h"


namespace dae
{

	class UggWrongWay final : public Component
	{
	public:
		explicit UggWrongWay(const std::shared_ptr<GameObject>& gameObject, int nrRows, float cubesWidth, float cubesHeight,
			float spriteWidth, float spriteHeight, int startingCube, float jumpInterval, bool isUgg, bool startingLeft);

		int GetPositionIndex() const { return m_CurrentCubeIdx; }
		bool GetIsAlive() const { return m_Alive; }
		bool GetStartedLeft() const { return m_StartingLeft; }
		int GetCurrentRow() const { return m_CurrentRow; }

		void ChangeTile() const;
		void Die() const;

		void Freeze(bool freeze);

		bool MoveLeftUp();
		bool MoveRightUp();
		bool MoveLeft();
		bool MoveRight();

		void Update() override;
		void Render() const override {};

	private:
		std::shared_ptr<dae::GameObject> m_GameObject{};
		int m_CurrentCubeIdx;
		int m_CurrentRow;
		int m_LastRow;
		float m_CubesWidth, m_CubesHeight;
		float m_SpriteWidth, m_SpriteHeight;
		float m_JumpTimer{ 0 };
		const float m_JumpInterval;
		bool m_Alive{ true };
		bool m_IsUgg, m_StartingLeft;
		bool m_IsFrozen{ false };
	};

}