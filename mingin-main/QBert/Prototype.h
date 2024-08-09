#pragma once
#include <memory>

const int g_NrRows = 7;
const float g_CubesActualWidth = 64.f;
const float g_CubesActualHeight = 64.f;
const int g_EnemiesLeftSpawnPosX = 285;
const float g_EnemiesRightSpawnPosX = 350;
const float g_EnemiesSpawnPosY = 110;

namespace dae
{
	class GameObject;
	std::shared_ptr<GameObject> MakeQBert();
	std::shared_ptr<GameObject> MakeSlickSam(bool isSlick, bool isLeft, float  moveInterval);
}