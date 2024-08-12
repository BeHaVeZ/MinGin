#pragma once
#include <memory>
#include <vector>

const int g_NrRows = 7;
const float g_CubesActualWidth = 64.f;
const float g_CubesActualHeight = 64.f;
const int g_EnemiesLeftSpawnPosX = 285;
const float g_EnemiesRightSpawnPosX = 350;
const float g_EnemiesSpawnPosY = 110;

namespace dae
{
	class GameObject;
	std::vector<std::shared_ptr<GameObject>> MakeQBert();
	std::shared_ptr<GameObject> MakeSlickSam(bool isSlick, bool isLeft, float  moveInterval);
	std::shared_ptr<GameObject> MakeUggWrongway(bool isUgg, bool isLeft, float moveInterval);
	std::shared_ptr<GameObject> MakeLevelTitle(int levelNumber);
	std::shared_ptr<GameObject> MakeLevelTransition();
	std::shared_ptr<GameObject> MakeWinScreen();
}