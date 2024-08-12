#pragma once
#include <memory>
#include "Component.h"

namespace dae
{
	class SlickSam final : public Component
	{
	public:
		explicit SlickSam(const std::shared_ptr<GameObject>& gameObject, int nrRows, float cubesWidth, float cubesHeight,
			float spriteWidth, float spriteHeight, int startingCube, float jumpInterval, bool isSlick);

		int GetPositionIndex() const { return m_CurrentCubeIdx; }

		void ChangeTile() const;

		bool GetIsAlive() const { return m_Alive; }

		void Freeze(bool freeze);
		void Die() const;

		bool MoveLeftDown();
		bool MoveRightDown();

		void Update() override;
		void Render() const override {};

	private:
		std::weak_ptr<GameObject> m_GameObject{};
		int m_CurrentCubeIdx;
		int m_CurrentRow{ 2 };
		int m_LastRow;
		float m_CubesWidth, m_CubesHeight;
		float m_SpriteWidth, m_SpriteHeight;
		float m_JumpTimer{ 0 };
		const float m_JumpInterval;
		bool m_Alive{ true };
		bool m_IsSlick;
		bool m_IsFrozen{ false };
	};
}