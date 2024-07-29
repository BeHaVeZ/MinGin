#include "Cube.h"
#include "GameObject.h"
#include "TextureComponent.h"

namespace dae
{
	Cube::Cube(const std::shared_ptr<GameObject>& gameObject)
		: m_GameObject(gameObject)
		, m_Turned(false)

	{
	}

	void Cube::TurnCube()
	{
		if (m_Turned == false)
		{
			if (auto gameObject = m_GameObject.lock())
			{
				gameObject->GetComponent<TextureComponent>()->SetTexture("Cube Yellow.png");
			}
			m_Turned = true;
		}
	}

	void Cube::Update()
	{
	}
}