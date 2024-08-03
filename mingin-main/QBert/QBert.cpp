// System includes
#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#define WIN32_LEAN_AND_MEAN

#include <stdexcept>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>
#include <mutex>
#include <utility>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <condition_variable>
#include <filesystem>

// SDL includes
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_keycode.h>
#include <SDL_events.h>
#include <SDL_mixer.h>

// GLM includes
#include <glm/glm.hpp>

// Project includes
#include "Minigin.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Component.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "FPSCounter.h"
#include "RotationComponent.h"
#include "Font.h"
#include "Timer.h"
#include "Transform.h"
#include "InputManager.h"
#include "Texture2D.h"
#include "GamePad.h"
#include "Input.h"
#include "Command.h"
#include "Observer.h"
#include "HealthComponent.h"
#include "Keyboard.h"
#include "ScoreComponent.h"
#include "QBertCharacter.h"
#include "Pyramid.h"
#include "QBertCommands.h"
#include "LevelObserver.h"

//////////////////////////////////////////// Logging macros
#define ANSI_COLOR_RESET "\033[0m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_WHITE "\033[37m"
#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_ORANGE "\033[33m"

#define LOG_TRACE(message) std::cout << ANSI_COLOR_GREEN << "[TRACE] " << message << ANSI_COLOR_RESET << std::endl
#define LOG_INFO(message) std::cout << ANSI_COLOR_WHITE << "[INFO] " << message << ANSI_COLOR_RESET << std::endl
#define LOG_CRITICAL(message) std::cerr << ANSI_COLOR_RED << "[CRITICAL] " << message << ANSI_COLOR_RESET << std::endl
#define LOG_WARNING(message) std::cout << ANSI_COLOR_ORANGE << "[WARNING] " << message << ANSI_COLOR_RESET << std::endl

using namespace dae;
namespace fs = std::filesystem;

const int g_NrRows = 7;
const float g_CubesActualWidth = 64.f;
const float g_CubesActualHeight = 64.f;
const int g_CubesSpriteWidth = 32;
const int g_CubesSpriteHeight = 32;
std::shared_ptr<GameObject> g_QBertGO;


void SetUpGlobalGameObjects();
void LoadFirstLvl();
void LoadSecondLvl();
void LoadThirdLvl();

void loadgame()
{
    SetUpGlobalGameObjects();
    LoadFirstLvl();
    LoadSecondLvl();
    LoadThirdLvl();

    //auto mainObserverGO = std::make_shared<GameObject>();
    //mainObserverGO->AddComponent<LevelObserver>(mainObserverGO, qBertGO->GetComponent<QBertCharacter>(), std::move(pyramid));
    //mainObserverGO->GetComponent<LevelObserver>()->Initialize();
    //scene.Add(mainObserverGO);
}

int main(int, char* []) {

    srand((unsigned)time(NULL));

    std::string dataPath = "../Data/";
    if (!fs::exists(dataPath) || !fs::is_directory(dataPath)) {
        dataPath = "../../Data/";
    }
    try {
        dae::Minigin engine(dataPath);
        engine.Run(loadgame);
    }
    catch (const std::exception& e) {
        LOG_CRITICAL(e.what());
        return EXIT_FAILURE;
    }
    catch (...) {
        LOG_CRITICAL("...");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

void SetUpGlobalGameObjects()
{
    float qBertSpriteWidth = 17.f;
    float qBertSpriteHeight = 17.f;
    auto qBertGO = std::make_shared<GameObject>();
    qBertGO->AddComponent<QBertCharacter>(qBertGO, g_NrRows, g_CubesActualWidth, g_CubesActualHeight, qBertSpriteWidth, qBertSpriteHeight);
    qBertGO->AddComponent<TextureComponent>("QBert Sprites.png", 304, 50, 49, 48, qBertSpriteWidth * 2, 0, qBertSpriteWidth, qBertSpriteHeight);

    auto bindCommand = [&qBertGO](SDL_Scancode key, std::shared_ptr<Command> command) {
        command->SetActor(qBertGO);
        Input::GetInstance().AddCommand(std::make_pair(key, SDL_KEYDOWN), command);
        };

    bindCommand(SDL_SCANCODE_W, std::make_shared<QBertMoveUpCommand>());
    bindCommand(SDL_SCANCODE_S, std::make_shared<QBertMoveDownCommand>());
    bindCommand(SDL_SCANCODE_A, std::make_shared<QBertMoveLeftCommand>());
    bindCommand(SDL_SCANCODE_D, std::make_shared<QBertMoveRightCommand>());

    g_QBertGO = std::move(qBertGO);
}

void LoadFirstLvl()
{
    using namespace dae;

    std::vector<std::shared_ptr<GameObject>> levelObservers(4);
    for (auto& observer : levelObservers) {
        observer = std::make_shared<GameObject>();
    }

    auto& scene1 = SceneManager::GetInstance().CreateScene("scene1A");

    auto pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight,
        0, 1, g_CubesSpriteWidth, g_CubesSpriteHeight);
    for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
        scene1.Add(cube);

    scene1.Add(g_QBertGO);

    levelObservers[0]->AddComponent<LevelObserver>(levelObservers[0], g_QBertGO->GetComponent<QBertCharacter>(), std::move(pyramid));
    scene1.Add(levelObservers[0]);

    scene1.Init();


    auto& scene2 = SceneManager::GetInstance().CreateScene("scene2A");

    pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight, 1, 1, g_CubesSpriteWidth, g_CubesSpriteHeight);
    for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
        scene2.Add(cube);

    scene2.Add(g_QBertGO);

    levelObservers[1]->AddComponent<LevelObserver>(levelObservers[1], g_QBertGO->GetComponent<QBertCharacter>(), std::move(pyramid));
    scene2.Add(levelObservers[1]);

    auto& scene3 = SceneManager::GetInstance().CreateScene("scene3A");

    pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight, 2, 1, g_CubesSpriteWidth, g_CubesSpriteHeight);
    for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
        scene3.Add(cube);

    scene3.Add(g_QBertGO);

    levelObservers[2]->AddComponent<LevelObserver>(levelObservers[2], g_QBertGO->GetComponent<QBertCharacter>(), std::move(pyramid));
    scene3.Add(levelObservers[2]);

    auto& scene4 = SceneManager::GetInstance().CreateScene("scene4A");

    pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight, 3, 1, g_CubesSpriteWidth, g_CubesSpriteHeight);
    for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
        scene4.Add(cube);

    scene4.Add(g_QBertGO);

    levelObservers[3]->AddComponent<LevelObserver>(levelObservers[3], g_QBertGO->GetComponent<QBertCharacter>(), std::move(pyramid));
    scene4.Add(levelObservers[3]);
}


void LoadSecondLvl()
{
    auto& scene1 = SceneManager::GetInstance().CreateScene("scene1B");

    auto pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight,
        1, 2, g_CubesSpriteWidth, g_CubesSpriteHeight);
    for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
        scene1.Add(cube);

    scene1.Add(g_QBertGO);

    auto levelObserverGO = std::make_shared<GameObject>();
    levelObserverGO->AddComponent<LevelObserver>(levelObserverGO, g_QBertGO->GetComponent<QBertCharacter>(), std::move(pyramid));
    scene1.Add(levelObserverGO);

    auto& scene2 = SceneManager::GetInstance().CreateScene("scene2B");

    pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight,
        3, 2, g_CubesSpriteWidth, g_CubesSpriteHeight);
    for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
        scene2.Add(cube);

    scene2.Add(g_QBertGO);

    levelObserverGO = std::make_shared<GameObject>();
    levelObserverGO->AddComponent<LevelObserver>(levelObserverGO, g_QBertGO->GetComponent<QBertCharacter>(), std::move(pyramid));
    scene2.Add(levelObserverGO);

    auto& scene3 = SceneManager::GetInstance().CreateScene("scene2C");

    pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight,0, 2, g_CubesSpriteWidth, g_CubesSpriteHeight);
    for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
        scene3.Add(cube);

    scene3.Add(g_QBertGO);

    levelObserverGO = std::make_shared<GameObject>();
    levelObserverGO->AddComponent<LevelObserver>(levelObserverGO, g_QBertGO->GetComponent<QBertCharacter>(), std::move(pyramid));
    scene3.Add(levelObserverGO);

    auto& scene4 = SceneManager::GetInstance().CreateScene("scene2D");

    pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight, 4, 2, g_CubesSpriteWidth, g_CubesSpriteHeight);
    for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
        scene4.Add(cube);

    scene4.Add(g_QBertGO);

    levelObserverGO = std::make_shared<GameObject>();
    levelObserverGO->AddComponent<LevelObserver>(levelObserverGO, g_QBertGO->GetComponent<QBertCharacter>(), std::move(pyramid));
    scene4.Add(levelObserverGO);
}

void LoadThirdLvl()
{
    auto& scene1 = SceneManager::GetInstance().CreateScene("scene3A");

    std::unique_ptr<Pyramid> pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight,5, 3, g_CubesSpriteWidth, g_CubesSpriteHeight);
    for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
        scene1.Add(cube);

    scene1.Add(g_QBertGO);

    auto levelObserverGO = std::make_shared<GameObject>();
    levelObserverGO->AddComponent<LevelObserver>(levelObserverGO, g_QBertGO->GetComponent<QBertCharacter>(), std::move(pyramid));
    scene1.Add(levelObserverGO);

    auto& scene2 = SceneManager::GetInstance().CreateScene("scene3B");

    pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight, 2, 3, g_CubesSpriteWidth, g_CubesSpriteHeight);
    for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
        scene2.Add(cube);

    scene2.Add(g_QBertGO);

    levelObserverGO = std::make_shared<GameObject>();
    levelObserverGO->AddComponent<LevelObserver>(levelObserverGO, g_QBertGO->GetComponent<QBertCharacter>(), std::move(pyramid));
    scene2.Add(levelObserverGO);

    auto& scene3 = SceneManager::GetInstance().CreateScene("scene3C");

    pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight,
        1, 3, g_CubesSpriteWidth, g_CubesSpriteHeight);
    for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
        scene3.Add(cube);

    scene3.Add(g_QBertGO);

    levelObserverGO = std::make_shared<GameObject>();
    levelObserverGO->AddComponent<LevelObserver>(levelObserverGO, g_QBertGO->GetComponent<QBertCharacter>(), std::move(pyramid));
    scene3.Add(levelObserverGO);

    auto& scene4 = SceneManager::GetInstance().CreateScene("scene3D");

    pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight, 0, 3, g_CubesSpriteWidth, g_CubesSpriteHeight);
    for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
        scene4.Add(cube);

    scene4.Add(g_QBertGO);

    levelObserverGO = std::make_shared<GameObject>();
    levelObserverGO->AddComponent<LevelObserver>(levelObserverGO, g_QBertGO->GetComponent<QBertCharacter>(), std::move(pyramid));
    scene4.Add(levelObserverGO);
}
