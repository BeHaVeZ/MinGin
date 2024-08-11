#include "UggWrongway.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include <iostream>
#include "Subject.h"
#include "Timer.h"

namespace dae
{

	UggWrongWay::UggWrongWay(const std::shared_ptr<dae::GameObject>& gameObject, int nrRows, float cubesWidth, float cubesHeight,
		float spriteWidth, float spriteHeight, int startingCube, float jumpInterval, bool isUgg, bool startingLeft)
		: m_GameObject(gameObject)
		, m_CurrentCubeIdx(startingCube)
		, m_CurrentRow(nrRows)
		, m_LastRow(nrRows)
		, m_CubesWidth(cubesWidth)
		, m_CubesHeight(cubesHeight)
		, m_SpriteWidth(spriteWidth)
		, m_SpriteHeight(spriteHeight)
		, m_JumpInterval(jumpInterval)
		, m_IsUgg(isUgg)
		, m_StartingLeft(startingLeft)
	{}

	void UggWrongWay::Die() const
	{
		if (m_IsUgg)
			std::cout << "Ugg died\n";
		else
			std::cout << "Wrongway died\n";

		m_GameObject->ClearComponents();
	}

	void UggWrongWay::Freeze(bool freeze)
	{
		m_IsFrozen = freeze;
	}

	bool UggWrongWay::MoveLeftUp()
	{
		if (m_CurrentCubeIdx != m_CurrentRow * (m_CurrentRow + 1) / 2 - m_CurrentRow + 1 && m_CurrentCubeIdx != 1)
		{
			m_CurrentCubeIdx = m_CurrentCubeIdx - m_CurrentRow;
			m_CurrentRow--;
			auto graphics = m_GameObject->GetComponent<TextureComponent>();
			graphics->SetPosition(graphics->GetPosX() - m_CubesWidth / 2.f, graphics->GetPosY() - m_CubesHeight * 0.75f);

			if (m_IsUgg)
				graphics->SetSrcRectangle(m_SpriteWidth * 3, 0, m_SpriteWidth, m_SpriteHeight);
			else
				graphics->SetSrcRectangle(m_SpriteWidth * 3, m_SpriteHeight, m_SpriteWidth, m_SpriteHeight);

			m_Subject->Notify(Event::UggWrongwayMove);
			return true;
		}
		else
		{
			m_Alive = false;
			m_Subject->Notify(dae::Event::UggWrongwayFell);
			return false;
		}
	}

	bool UggWrongWay::MoveRightUp()
	{
		if (m_CurrentCubeIdx != m_CurrentRow * (m_CurrentRow + 1) / 2)
		{
			m_CurrentCubeIdx = m_CurrentCubeIdx - m_CurrentRow + 1;
			m_CurrentRow--;
			auto graphics = m_GameObject->GetComponent<TextureComponent>();
			graphics->SetPosition(graphics->GetPosX() + m_CubesWidth / 2.f, graphics->GetPosY() - m_CubesHeight * 0.75f);

			if (m_IsUgg)
				graphics->SetSrcRectangle(m_SpriteWidth, 0, m_SpriteWidth, m_SpriteHeight);
			else
				graphics->SetSrcRectangle(m_SpriteWidth, m_SpriteHeight, m_SpriteWidth, m_SpriteHeight);

			m_Subject->Notify(Event::UggWrongwayMove);
			return true;
		}
		else
		{
			m_Alive = false;
			m_Subject->Notify(dae::Event::UggWrongwayFell);
			return false;
		}
	}

	bool UggWrongWay::MoveLeft()
	{
		if (m_CurrentCubeIdx != m_CurrentRow * (m_CurrentRow + 1) / 2 - m_CurrentRow + 1 && m_CurrentCubeIdx != 1)
		{
			m_CurrentCubeIdx = m_CurrentCubeIdx - 1;
			auto graphics = m_GameObject->GetComponent<TextureComponent>();
			graphics->SetPosition(graphics->GetPosX() - m_CubesWidth, graphics->GetPosY());

			if (m_IsUgg)
				graphics->SetSrcRectangle(m_SpriteWidth * 2, 0, m_SpriteWidth, m_SpriteHeight);
			else
				graphics->SetSrcRectangle(m_SpriteWidth * 2, m_SpriteHeight, m_SpriteWidth, m_SpriteHeight);

			m_Subject->Notify(dae::Event::UggWrongwayMove);
			return true;
		}
		else
		{
			m_Alive = false;
			m_Subject->Notify(dae::Event::UggWrongwayFell);
			return false;
		}
	}

	bool UggWrongWay::MoveRight()
	{
		if (m_CurrentCubeIdx != m_CurrentRow * (m_CurrentRow + 1) / 2)
		{
			m_CurrentCubeIdx = m_CurrentCubeIdx + 1;
			auto graphics = m_GameObject->GetComponent<TextureComponent>();
			graphics->SetPosition(graphics->GetPosX() + m_CubesWidth, graphics->GetPosY());

			if (m_IsUgg)
				graphics->SetSrcRectangle(0, 0, m_SpriteWidth, m_SpriteHeight);
			else
				graphics->SetSrcRectangle(0, m_SpriteHeight, m_SpriteWidth, m_SpriteHeight);

			m_Subject->Notify(dae::Event::UggWrongwayMove);
			return true;
		}
		else
		{
			m_Alive = false;
			m_Subject->Notify(dae::Event::UggWrongwayFell);
			return false;
		}
	}

	void UggWrongWay::Update()
	{
		if (m_Alive)
		{
			m_JumpTimer += Timer::GetInstance().GetDeltaTime();

			if (m_JumpTimer >= m_JumpInterval)
			{
				if ((rand() % 2) + 1 == 1)
				{
					if (m_StartingLeft)
						MoveRightUp();
					else
						MoveLeftUp();
				}
				else
				{
					if (m_StartingLeft)
						MoveRight();
					else
						MoveLeft();
				}

				m_JumpTimer -= m_JumpInterval;
			}
		}
	}

}