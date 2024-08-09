#include "Prototype.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "SceneManager.h"
#include "SlickSam.h"
#include "Scene.h"
#include "QBertCharacter.h"


namespace dae
{

	std::shared_ptr<GameObject> MakeSlickSam(bool isSlick, bool isLeft, float  moveInterval)
	{
		const auto spriteWidth = 12.f;
		const auto spriteHeight = 16.f;
		const auto actualWidth = 28.f;
		const auto actualHeight = 33.f;

		auto newGO = std::make_shared<GameObject>();

		if (isSlick)
		{
			if (isLeft)
			{
				newGO->AddComponent<SlickSam>(newGO, g_NrRows, g_CubesActualWidth, g_CubesActualHeight, spriteWidth, spriteHeight, 2, moveInterval, true);
				newGO->AddComponent<TextureComponent>("SlickSam Spritesheet.png", g_EnemiesLeftSpawnPosX, g_EnemiesSpawnPosY, actualWidth, actualHeight, 0, 0, spriteWidth, spriteHeight);
			}
			else
			{
				newGO->AddComponent<SlickSam>(newGO, g_NrRows, g_CubesActualWidth, g_CubesActualHeight, spriteWidth, spriteHeight, 3, moveInterval, true);
				newGO->AddComponent<TextureComponent>("SlickSam Spritesheet.png", g_EnemiesRightSpawnPosX, g_EnemiesSpawnPosY, actualWidth, actualHeight, 0, 0, spriteWidth, spriteHeight);
			}
		}
		else
		{
			if (isLeft)
			{
				newGO->AddComponent<SlickSam>(newGO, g_NrRows, g_CubesActualWidth, g_CubesActualHeight, spriteWidth, spriteHeight, 2, moveInterval, false);
				newGO->AddComponent<TextureComponent>("SlickSam Spritesheet.png", g_EnemiesLeftSpawnPosX, g_EnemiesSpawnPosY, actualWidth, actualHeight, spriteWidth, spriteHeight, spriteWidth, spriteHeight);
			}
			else
			{
				newGO->AddComponent<SlickSam>(newGO, g_NrRows, g_CubesActualWidth, g_CubesActualHeight, spriteWidth, spriteHeight, 3, moveInterval, false);
				newGO->AddComponent<TextureComponent>("SlickSam Spritesheet.png", g_EnemiesRightSpawnPosX, g_EnemiesSpawnPosY, actualWidth, actualHeight, spriteWidth, spriteHeight, spriteWidth, spriteHeight);
			}
		}

		return newGO;
	}


	std::shared_ptr<GameObject> MakeQBert()
	{
		const auto spriteWidth = 17.f;
		const auto spriteHeight = 17.f;
		const auto initialPosX = 304.f;
		const auto initialPosY = 50.f;
		const auto actualWidth = 49.f;
		const auto actualHeight = 48.f;

		auto qBertGO = std::make_shared<GameObject>();
		qBertGO->AddComponent<QBertCharacter>(qBertGO, g_NrRows, g_CubesActualWidth, g_CubesActualHeight, spriteWidth, spriteHeight);
		qBertGO->AddComponent<TextureComponent>("Qbert Sprites.png", initialPosX, initialPosY, actualWidth, actualHeight, spriteWidth * 2, 0, spriteWidth, spriteHeight);

		return qBertGO;
	}

}