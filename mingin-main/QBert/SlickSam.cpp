#include "SlickSam.h"

#include <iostream>

#include "GameObject.h"
#include "TextureComponent.h"

#include "Subject.h"
#include "Timer.h"

namespace dae
{


	SlickSam::SlickSam(const std::shared_ptr<GameObject>& gameObject, int nrRows, float cubesWidth, float cubesHeight,
		float spriteWidth, float spriteHeight, int startingCube, float jumpInterval, bool isSlick)
		: m_GameObject(gameObject)
		, m_CurrentCubeIdx(startingCube)
		, m_LastRow(nrRows)
		, m_CubesWidth(cubesWidth)
		, m_CubesHeight(cubesHeight)
		, m_SpriteWidth(spriteWidth)
		, m_SpriteHeight(spriteHeight)
		, m_JumpInterval(jumpInterval)
		, m_IsSlick(isSlick)
	{}

	void SlickSam::Die() const
	{
		m_GameObject.lock()->ClearComponents();
	}

	void SlickSam::Freeze(bool freeze)
	{
		m_IsFrozen = freeze;
	}

	bool SlickSam::MoveLeftDown()
	{
		if (m_IsFrozen == false)
		{
			if (m_CurrentRow != m_LastRow)
			{
				m_CurrentCubeIdx = m_CurrentCubeIdx + m_CurrentRow;
				m_CurrentRow++;
				auto graphics = m_GameObject.lock()->GetComponent<TextureComponent>();
				graphics->SetPosition(graphics->GetPosX() - m_CubesWidth / 2.f, graphics->GetPosY() + m_CubesHeight * 0.75f);

				if (m_IsSlick)
					graphics->SetSrcRectangle(0, 0, m_SpriteWidth, m_SpriteHeight);
				else
					graphics->SetSrcRectangle(0, m_SpriteHeight, m_SpriteWidth, m_SpriteHeight);

				m_Subject->Notify(Event::SlickSamMoved);
				return true;
			}
			else
			{
				m_Alive = false;
				m_Subject->Notify(Event::SlickSamFell);
				return false;
			}
		}
		return false;
	}

	bool SlickSam::MoveRightDown()
	{
		if (m_IsFrozen == false)
		{
			if (m_CurrentRow != m_LastRow)
			{
				m_CurrentCubeIdx = m_CurrentCubeIdx + m_CurrentRow + 1;
				m_CurrentRow++;
				auto graphics = m_GameObject.lock()->GetComponent<TextureComponent>();
				graphics->SetPosition(graphics->GetPosX() + m_CubesWidth / 2.f, graphics->GetPosY() + m_CubesHeight * 0.75f);

				if (m_IsSlick)
					graphics->SetSrcRectangle(m_SpriteWidth, 0, m_SpriteWidth, m_SpriteHeight);
				else
					graphics->SetSrcRectangle(m_SpriteWidth, m_SpriteHeight, m_SpriteWidth, m_SpriteHeight);

				m_Subject->Notify(Event::SlickSamMoved);
				return true;
			}
			else
			{
				m_Alive = false;
				m_Subject->Notify(Event::SlickSamFell);
				return false;
			}
		}
		return false;
	}

	void SlickSam::Update()
	{
		if (m_Alive and !m_IsFrozen)
		{
			m_JumpTimer += Timer::GetInstance().GetDeltaTime();

			if (m_JumpTimer >= m_JumpInterval)
			{

				if ((rand() % 2) + 1 == 1)
					MoveRightDown();
				else
					MoveLeftDown();

				m_JumpTimer -= m_JumpInterval;
			}
		}
	}
}
