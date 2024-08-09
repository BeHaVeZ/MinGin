#include "Cube.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include <iostream>

namespace dae
{
	Cube::Cube(const std::shared_ptr<GameObject>& gameObject, int colorIdx, int level, float cubeSpriteWidth, float cubeSpriteHeight)
		: m_GameObject(gameObject),
		m_Turned{ false },
		m_HalfTurned{ false },
		m_ColorIdx{ colorIdx },
		m_Level{ level },
		m_CubeSpriteWidth{ cubeSpriteWidth },
		m_CubeSpriteHeight{ cubeSpriteHeight }
	{
	}

	void Cube::TurnCube()
	{
		auto gameObject = m_GameObject.lock();
		if (!gameObject)
			return;

		if (!m_Turned)
		{
			if (m_Level == 1)
			{
				gameObject->GetComponent<TextureComponent>()->SetSrcRectangle(float(m_ColorIdx) * m_CubeSpriteWidth, m_CubeSpriteHeight, m_CubeSpriteWidth, m_CubeSpriteHeight);
				m_Turned = true;
			}
			else if (m_Level == 2)
			{
				if (m_HalfTurned)
				{
					gameObject->GetComponent<TextureComponent>()->SetSrcRectangle(float(m_ColorIdx) * m_CubeSpriteWidth, m_CubeSpriteHeight, m_CubeSpriteWidth, m_CubeSpriteHeight);
					m_Turned = true;
				}
				else
				{
					gameObject->GetComponent<TextureComponent>()->SetSrcRectangle(float(m_ColorIdx) * m_CubeSpriteWidth, m_CubeSpriteHeight * 2, m_CubeSpriteWidth, m_CubeSpriteHeight);
					m_HalfTurned = true;
				}
			}
			else
			{
				gameObject->GetComponent<TextureComponent>()->SetSrcRectangle(float(m_ColorIdx) * m_CubeSpriteWidth, m_CubeSpriteHeight, m_CubeSpriteWidth, m_CubeSpriteHeight);
				m_Turned = true;
			}
		}
		else
		{
			if (m_Level == 3)
			{
				gameObject->GetComponent<TextureComponent>()->SetSrcRectangle(float(m_ColorIdx) * m_CubeSpriteWidth, 0, m_CubeSpriteWidth, m_CubeSpriteHeight);
				m_Turned = false;
			}
		}
	}

	void Cube::SlickSamTurnCube()
	{
		auto gameObject = m_GameObject.lock();

		if (!gameObject) return;


		if (m_Turned)
		{
			if (m_Level == 1)
			{
				gameObject->GetComponent<TextureComponent>()->SetSrcRectangle(float(m_ColorIdx) * m_CubeSpriteWidth, 0, m_CubeSpriteWidth, m_CubeSpriteHeight);
			}
			else if (m_Level == 2)
			{
				gameObject->GetComponent<TextureComponent>()->SetSrcRectangle(float(m_ColorIdx) * m_CubeSpriteWidth, m_CubeSpriteHeight, m_CubeSpriteWidth, m_CubeSpriteHeight);
				m_HalfTurned = true;
			}
			else
			{
				gameObject->GetComponent<TextureComponent>()->SetSrcRectangle(float(m_ColorIdx) * m_CubeSpriteWidth, 0, m_CubeSpriteWidth, m_CubeSpriteHeight);
			}

			m_Turned = false;
		}
		else
		{
			if (m_Level == 2)
			{
				if (m_HalfTurned)
				{
					gameObject->GetComponent<TextureComponent>()->SetSrcRectangle(float(m_ColorIdx) * m_CubeSpriteWidth, 0, m_CubeSpriteWidth, m_CubeSpriteHeight);
					m_HalfTurned = false;
				}
			}
		}
	}

	void Cube::Update()
	{
	}
}