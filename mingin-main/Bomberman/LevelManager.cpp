#include "LevelManager.h"
#include "GameObject.h"
#include "BoxCollider.h"

#define CREATE_GAMEOBJECT(...) std::make_shared<dae::GameObject>(__VA_ARGS__)

LevelManager::LevelManager(float cellSize, int gridWidth, int gridHeight)
    : m_CellSize(cellSize), m_GridWidth(gridWidth), m_GridHeight(gridHeight)
{
}

void LevelManager::CreateLevel(dae::Scene& scene)
{
    for (int y = 0; y < m_GridHeight; ++y)
    {
        for (int x = 0; x < m_GridWidth; ++x)
        {
            if (x == 0 || x == m_GridWidth - 1 || y == 0 || y == m_GridHeight - 1)
            {
                AddBlockToScene(scene, x * m_CellSize, y * m_CellSize);
            }
            else if (x % 2 == 0 && y % 2 == 0)
            {
                AddBlockToScene(scene, x * m_CellSize, y * m_CellSize);
            }
            else
            {
                //AddTriggerToScene(scene, x * m_CellSize, y * m_CellSize);
            }
        }
    }
}

void LevelManager::AddBlockToScene(dae::Scene& scene, float x, float y)
{
    auto block = CREATE_GAMEOBJECT();
    block->AddComponent<dae::BoxCollider>(m_CellSize, m_CellSize, block.get());
    block->SetStatic(true);
    block->SetPosition(x, y);
    scene.Add(block);
}
