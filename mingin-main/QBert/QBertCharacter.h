#pragma once
#include <memory>
#include "Component.h"

namespace dae
{
	class Observer;
	class QBertCharacter final : public Component
	{
	public:

		explicit QBertCharacter(const std::shared_ptr<GameObject>& gameObject, const std::shared_ptr<GameObject>& cursesGameObject,int nrRows, float cubesWidth, float cubesHeight, float qBertSpriteWidth, float qBertSpriteHeight);

		virtual void Initialize() override;

		int GetCurrentLives() const { return m_Lives; }
		int GetPositionIndex() const { return m_CurrentCubeIdx; }

		void ChangeTile() const;
		void ChangeColor() const;
		void Die();
		void ResetPosition();
		void Freeze(bool freeze);
		void HideTexture(bool toHide);
		void HideCurses(bool toHide) const;

		bool MoveRightUp();
		bool MoveLeftDown();
		bool MoveLeftUp();
		bool MoveRightDown();

		virtual void Update() override;
		virtual void Render() const override {};

	private:
		int m_Lives{ 3 };
		int m_HP{ 100 };
		std::weak_ptr<GameObject> m_GameObject;
		std::weak_ptr<GameObject> m_CursesGameObject{};
		int m_CurrentCubeIdx{ 1 };
		int m_CurrentRow{ 1 };
		int m_LastRow;
		float m_CubesWidth, m_CubesHeight;
		float m_QBertSpriteWidth, m_QBertSpriteHeight;
		float m_QBertInitialPosX, m_QBertInitialPosY;
		bool m_Frozen{ true };
		float m_PosXBeforeHidden, m_PosYBeforeHidden;
		bool m_IsShowingTexture;
	};
}
