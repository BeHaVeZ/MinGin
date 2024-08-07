#include "QBertCharacter.h"
#include <iostream>
#include "GameObject.h"
#include "TextureComponent.h"
#include "Subject.h"


namespace dae
{
	QBertCharacter::QBertCharacter(const std::shared_ptr<GameObject>& gameObject, int nrRows, float cubesWidth, float cubesHeight, float qBertSpriteWidth, float qBertSpriteHeight)
		: m_GameObject(gameObject)
		, m_LastRow(nrRows)
		, m_CubesWidth(cubesWidth)
		, m_CubesHeight(cubesHeight)
		, m_QBertSpriteWidth(qBertSpriteWidth)
		, m_QBertSpriteHeight(qBertSpriteHeight)
		, m_QBertInitialPosX()
		, m_QBertInitialPosY()
	{
	}

	void QBertCharacter::Initialize()
	{
		auto texture = m_GameObject.lock()->GetComponent<TextureComponent>();
		m_QBertInitialPosX = texture->GetPosX();
		m_QBertInitialPosY = texture->GetPosY();
	}

	void QBertCharacter::ChangeTile() const
	{
		m_Subject->Notify(Event::TileChange);
	}

	void QBertCharacter::ChangeColor() const
	{
		m_Subject->Notify(Event::ColorChange);
	}

	void QBertCharacter::Die()
	{
		if (m_Lives - 1 >= 0)
		{
			m_Lives--;
			m_Subject->Notify(Event::ActorDeath);
		}
	}

	void QBertCharacter::ResetPosition()
	{
		auto gameObject = m_GameObject.lock();

		if (!gameObject)
			return;
		gameObject->GetComponent<TextureComponent>()->SetPosition(m_QBertInitialPosX, m_QBertInitialPosY);
		gameObject->GetComponent<TextureComponent>()->SetSrcRectangle(m_QBertSpriteWidth * 2, 0, m_QBertSpriteWidth, m_QBertSpriteHeight);

		m_CurrentCubeIdx = 1;
		m_CurrentRow = 1;
	}

	bool QBertCharacter::MoveUp()
	{
		if (m_CurrentCubeIdx != m_CurrentRow * (m_CurrentRow + 1) / 2)
		{
			m_CurrentCubeIdx = m_CurrentCubeIdx - m_CurrentRow + 1;
			m_CurrentRow--;
			m_Subject->Notify(Event::QBertMove);

			if (auto gameObject = m_GameObject.lock())
			{
				auto texture = gameObject->GetComponent<TextureComponent>();
				texture->SetPosition(texture->GetPosX() + m_CubesWidth / 2.f, texture->GetPosY() - m_CubesHeight * 0.75f);
				texture->SetSrcRectangle(0, 0, m_QBertSpriteWidth, m_QBertSpriteHeight);
			}
			return true;
		}

		return false;
	}

	bool QBertCharacter::MoveDown()
	{
		if (m_CurrentRow != m_LastRow)
		{
			m_CurrentCubeIdx = m_CurrentCubeIdx + m_CurrentRow;
			m_CurrentRow++;
			m_Subject->Notify(Event::QBertMove);

			if (auto gameObject = m_GameObject.lock())
			{
				auto texture = gameObject->GetComponent<TextureComponent>();
				texture->SetPosition(texture->GetPosX() - m_CubesWidth / 2.f, texture->GetPosY() + m_CubesHeight * 0.75f);
				texture->SetSrcRectangle(m_QBertSpriteWidth * 3, 0, m_QBertSpriteWidth, m_QBertSpriteHeight);
			}
			return true;
		}

		return false;
	}

	bool QBertCharacter::MoveLeft()
	{
		if (m_CurrentCubeIdx != m_CurrentRow * (m_CurrentRow + 1) / 2 - m_CurrentRow + 1 && m_CurrentCubeIdx != 1)
		{
			m_CurrentCubeIdx = m_CurrentCubeIdx - m_CurrentRow;
			m_CurrentRow--;
			m_Subject->Notify(Event::QBertMove);

			if (auto gameObject = m_GameObject.lock())
			{
				auto texture = gameObject->GetComponent<TextureComponent>();
				texture->SetPosition(texture->GetPosX() - m_CubesWidth / 2.f, texture->GetPosY() - m_CubesHeight * 0.75f);
				texture->SetSrcRectangle(m_QBertSpriteWidth, 0, m_QBertSpriteWidth, m_QBertSpriteHeight);
			}
			return true;
		}

		return false;
	}

	bool QBertCharacter::MoveRight()
	{
		if (m_CurrentRow != m_LastRow)
		{
			m_CurrentCubeIdx = m_CurrentCubeIdx + m_CurrentRow + 1;
			m_CurrentRow++;
			m_Subject->Notify(Event::QBertMove);

			if (auto gameObject = m_GameObject.lock())
			{
				auto texture = gameObject->GetComponent<TextureComponent>();
				texture->SetPosition(texture->GetPosX() + m_CubesWidth / 2.f, texture->GetPosY() + m_CubesHeight * 0.75f);
				texture->SetSrcRectangle(m_QBertSpriteWidth * 2, 0, m_QBertSpriteWidth, m_QBertSpriteHeight);
			}
			return true;
		}

		return false;
	}

	void QBertCharacter::Update()
	{
	}
}