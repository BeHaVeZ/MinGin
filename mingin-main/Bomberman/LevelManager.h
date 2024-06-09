#pragma once

#include "Scene.h"

class LevelManager
{
public:
    LevelManager(float cellSize, int gridWidth, int gridHeight);

    void CreateLevel(dae::Scene& scene);

private:
    float m_CellSize;
    int m_GridWidth;
    int m_GridHeight;

    void AddBlockToScene(dae::Scene& scene, float x, float y);
    void AddTriggerToScene(dae::Scene& scene, float x, float y);
};

