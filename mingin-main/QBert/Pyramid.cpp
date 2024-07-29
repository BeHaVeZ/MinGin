#include "Pyramid.h"
#include "QBertCharacter.h"
#include "TextureComponent.h"
#include "GameObject.h"
#include "Cube.h"


namespace dae
{
	Pyramid::Pyramid(float posX, float posY, int nrRows, float cubesWidth, float cubesHeight)
		: m_PosX{ posX }
		, m_PosY{ posY }
		, m_CubesWidth{ cubesWidth }
		, m_CubesHeight{ cubesHeight }
		, m_TotalCubes{ nrRows * (nrRows + 1) / 2 }
		, m_NrRows{ nrRows }

	{
		FillCubesGOVector();
	}

	void Pyramid::FillCubesGOVector()
	{
		float tempPosX = m_PosX;
		float tempPosY = m_PosY;
		int nrRow = 1;
		int cubesInRow = 0;

		for (int i = 0; i < m_TotalCubes; i++)
		{
			auto newCubeGO = std::make_shared<GameObject>();
			newCubeGO->AddComponent<Cube>(newCubeGO);
			newCubeGO->AddComponent<TextureComponent>("Cube Blue.png", tempPosX, tempPosY, m_CubesWidth, m_CubesHeight);

			m_CubeGOVector.push_back(std::move(newCubeGO));
			cubesInRow++;

			tempPosX += m_CubesWidth;

			if (cubesInRow >= nrRow)
			{
				tempPosY += m_CubesHeight * 0.75f;
				tempPosX = m_PosX - (m_CubesWidth / 2 * nrRow);
				cubesInRow = 0;
				nrRow++;
			}
		}
	}

}