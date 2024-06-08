#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#define WIN32_LEAN_AND_MEAN

///////TODO LINK THE CORRECT PCH.H from the lib by pre-build linking in command!!!!
////////////////////////////////////////////
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <windows.h>
#include <SDL_keycode.h>
#include <SDL_events.h>
#include <SDL_mixer.h>

////////////////////////////////////////////
#include <stdexcept>

////////////////////////////////////////////
#include <glm/glm.hpp>

////////////////////////////////////////////
#include <filesystem>
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

////////////////////////////////////////////
#include "Minigin.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Component.h"
#include "GameObject.h"
#include "TextObject.h"
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
#include "Event.h"
#include "HealthComponent.h"
#include "Keyboard.h"
#include "ScoreComponent.h"
#include "ServiceLocator.h"
#include "BoxCollider.h"
#include "LevelManager.h"

////////////////////////////////////////////
#define CREATE_GAMEOBJECT(...) std::make_shared<dae::GameObject>(__VA_ARGS__)

namespace fs = std::filesystem;

enum class Sounds : unsigned short
{
    Explosion = 0,
    MainMenu
};

// Constants for grid and cell size
const float CELL_SIZE = 48.f;
const int GRID_WIDTH = 31;
const int GRID_HEIGHT = 13;
const float SCREEN_WIDTH = 640.f;
const float SCREEN_HEIGHT = 480.f;

void AddBlockToScene(dae::Scene& scene, float x, float y)
{
    auto block = CREATE_GAMEOBJECT();
    block->AddComponent<dae::BoxCollider>(CELL_SIZE, CELL_SIZE, block.get());
    block->SetStatic(true);
    block->SetPosition(x, y);
    scene.Add(block);
}

void load()
{
    using namespace dae;
    auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

    // Decorator
#if _DEBUG
    ServiceLocator::RegisterSoundSystem(new Logging_SoundSystem(new SDL_SoundSystem()));
#else
    ServiceLocator::RegisterSoundSystem(new SDL_SoundSystem());
#endif

    ServiceLocator::GetSoundSystem().AddSound("BombExplodes.wav", (unsigned short)Sounds::Explosion, false);
    ServiceLocator::GetSoundSystem().AddSound("Main.wav", (unsigned short)Sounds::MainMenu, true);
    ServiceLocator::GetSoundSystem().PlaySound((unsigned short)Sounds::MainMenu, 50.f);

    auto go = CREATE_GAMEOBJECT();
    go->AddComponent<TextureComponent>("Playfield.png");
    go->GetTransform().SetPosition(1.f, 1.f, 1.f);
    scene.Add(go);

    auto font = ResourceManager::GetInstance().LoadFont("bb.otf", 12);

    auto fpsObject = CREATE_GAMEOBJECT();
    fpsObject->AddComponent<FPSCounter>(font);
    scene.Add(fpsObject);


    auto player1 = CREATE_GAMEOBJECT();
    player1->AddComponent<TextureComponent>("bomberman.png");
    player1->GetTransform().SetScale(2.5f, 2.5f, 1.f);
    player1->AddComponent<HealthComponent>(4);
    player1->AddComponent<ScoreComponent>();
    player1->AddComponent<BoxCollider>(30.f, 40.f, player1.get());
    player1->SetPosition(50.f, 50.f);

    LevelManager levelManager(CELL_SIZE, GRID_WIDTH, GRID_HEIGHT);
    levelManager.CreateLevel(scene);

    auto textObjectLives = CREATE_GAMEOBJECT();
    textObjectLives->AddComponent<TextObject>("Lives player 1: 3", font);
    textObjectLives->SetPosition(50.f, 300.f);
    scene.Add(textObjectLives);

    auto textObjectLivesComponent = textObjectLives->GetComponent<TextObject>();
    auto weakHealthComponent = player1->GetComponent<HealthComponent>();

    weakHealthComponent->GetOnDeathEvent()->AddListener([textObjectLivesComponent](int lives) {
        textObjectLivesComponent->SetText("Lives: " + std::to_string(lives));
        });

    auto textObjectScore = CREATE_GAMEOBJECT();
    textObjectScore->AddComponent<TextObject>("Score: 0", font);
    textObjectScore->SetPosition(50.f, 350.f);
    scene.Add(textObjectScore);

    auto textObjectScoreComponent = textObjectScore->GetComponent<TextObject>();
    auto weakScoreComponent = player1->GetComponent<ScoreComponent>();

    weakScoreComponent->GetOnScoreChangedEvent()->AddListener([textObjectScoreComponent](int score) {
        textObjectScoreComponent->SetText("Score: " + std::to_string(score));
        });

    const float moveSpeed{ 20.f };

    auto killCommand = std::make_shared<KillCommand>(player1->GetComponent<HealthComponent>());
    Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_E, SDL_KEYDOWN), killCommand);
    auto addScoreCommand = std::make_shared<AddScoreCommand>(player1->GetComponent<ScoreComponent>());
    Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_R, SDL_KEYDOWN), addScoreCommand);

    Input::GetInstance().AddController(std::make_shared<GamePad>(0));

    auto moveCommand = std::make_shared<MoveCommand>(player1, glm::vec3{ 0.f, -1.f, 0.f }, moveSpeed * 4.f);
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::DPadUp, KeyState::Pressed), moveCommand);

    moveCommand = std::make_shared<MoveCommand>(player1, glm::vec3{ 0.f, 1.f, 0.f }, moveSpeed * 4.f);
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::DPadDown, KeyState::Pressed), moveCommand);

    moveCommand = std::make_shared<MoveCommand>(player1, glm::vec3{ -1.f, 0.f, 0.f }, moveSpeed * 4.f);
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::DPadLeft, KeyState::Pressed), moveCommand);

    moveCommand = std::make_shared<MoveCommand>(player1, glm::vec3{ 1.f, 0.f, 0.f }, moveSpeed * 4.f);
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::DPadRight, KeyState::Pressed), moveCommand);

    killCommand = std::make_shared<KillCommand>(player1->GetComponent<HealthComponent>());
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::ButtonSouth, KeyState::Down), killCommand);

    addScoreCommand = std::make_shared<AddScoreCommand>(player1->GetComponent<ScoreComponent>());
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::ButtonWest, KeyState::Down), addScoreCommand);

    scene.Add(player1);
}

int main(int, char* [])
{
    std::string dataPath = "../Data/";
    if (fs::exists(dataPath) && fs::is_directory(dataPath))
    {
    }
    else
    {
        dataPath = "../../Data/";
    }
    try
    {
        dae::Minigin engine(dataPath);
        engine.Run(load);
    }
    catch (...)
    {
        std::cerr << ("Ja kijk maar nog is na waar je data folder staat.");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
