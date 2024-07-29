#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#define WIN32_LEAN_AND_MEAN


////////////////////////////////////////////
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <windows.h>
#include <SDL_keycode.h>
#include <SDL_events.h>
#include <SDL_Mixer.h>


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
#include <queue>
#include <condition_variable>

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
#include "Observer.h"
#include "HealthComponent.h"
#include "Keyboard.h"
#include "ScoreComponent.h"
#include "QBertCharacter.h"
#include "Pyramid.h"

////////////////////////////////////////////
#define CREATE_GAMEOBJECT(...) std::make_shared<dae::GameObject>(__VA_ARGS__)


////////////////////////////////////////////
#define ANSI_COLOR_RESET "\033[0m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_WHITE "\033[37m"
#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_ORANGE "\033[33m"


#define LOG_TRACE(message) std::cout << ANSI_COLOR_GREEN << "[TRACE] " << message << ANSI_COLOR_RESET << std::endl
#define LOG_INFO(message) std::cout << ANSI_COLOR_WHITE << "[INFO] " << message << ANSI_COLOR_RESET << std::endl
#define LOG_CRITICAL(message) std::cerr << ANSI_COLOR_RED << "[CRITICAL] " << message << ANSI_COLOR_RESET << std::endl
#define LOG_WARNING(message) std::cout << ANSI_COLOR_ORANGE << "[WARNING] " << message << ANSI_COLOR_RESET << std::endl


#include "Minigin.h"

namespace fs = std::filesystem;

void loadgame() 
{
    using namespace dae;

    const int nrRows = 7;
    const float cubesWidth = 56.f;
    const float cubesHeight = 56.f;

    auto& scene = dae::SceneManager::GetInstance().CreateScene("Game");

    auto pyramid = std::make_unique<Pyramid>(300.f, 80.f, nrRows, cubesWidth, cubesHeight);

    for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
        scene.Add(cube);

}

void load()
{
    using namespace dae;
    auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

    auto go = CREATE_GAMEOBJECT();
    go->AddComponent<TextureComponent>("background.tga");
    scene.Add(go);

    go = CREATE_GAMEOBJECT();
    go->AddComponent<TextureComponent>("logo.tga");
    go->SetPosition(216, 180);
    scene.Add(go);

    auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    auto to = CREATE_GAMEOBJECT();
    to->AddComponent<TextObject>("Programming 4 Assignment", font);
    to->GetComponent<TextObject>()->SetPosition(20, 20);
    to->SetPosition(60, 20);
    scene.Add(to);

    auto fpsObject = CREATE_GAMEOBJECT();
    fpsObject->AddComponent<FPSCounter>(font);
    scene.Add(fpsObject);


    auto player1 = CREATE_GAMEOBJECT();
    player1->AddComponent<TextureComponent>("qBert.png");
    player1->AddComponent<HealthComponent>(3);
    player1->AddComponent<ScoreComponent>();
    player1->SetPosition(100.f, 100.f);




    scene.Add(player1);

    const float moveSpeed{ 20.f };
    auto moveCommand = std::make_shared<MoveCommand>(player1, glm::lowp_vec3{ 0.f,-1.f,0.f }, moveSpeed);
    Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_W, SDL_KEYMAPCHANGED), moveCommand);

    moveCommand = std::make_shared<MoveCommand>(player1, glm::vec3{ -1.f, 0.f, 0.f }, moveSpeed);
    Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_A, SDL_KEYMAPCHANGED), moveCommand);

    moveCommand = std::make_shared<MoveCommand>(player1, glm::vec3{ 0.f, 1.f, 0.f }, moveSpeed);
    Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_S, SDL_KEYMAPCHANGED), moveCommand);

    moveCommand = std::make_shared<MoveCommand>(player1, glm::vec3{ 1.f, 0.f, 0.f }, moveSpeed);
    Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_D, SDL_KEYMAPCHANGED), moveCommand);



    auto player2 = CREATE_GAMEOBJECT();
    player2->AddComponent<TextureComponent>("Qbert General Sprites.png");
    player2->AddComponent<HealthComponent>(3);
    player2->AddComponent<ScoreComponent>();
    player2->SetPosition(200.f, 200.f);

    auto textObjectLivesPlayer2 = CREATE_GAMEOBJECT();
    textObjectLivesPlayer2->AddComponent<TextObject>("Lives player 2: 3", font);
    textObjectLivesPlayer2->SetPosition(350, 300.f);
    scene.Add(textObjectLivesPlayer2);

    auto textObjectScorePlayer2 = CREATE_GAMEOBJECT();
    textObjectScorePlayer2->AddComponent<TextObject>("Score: 0", font);
    textObjectScorePlayer2->SetPosition(350.f, 350.f);
    scene.Add(textObjectScorePlayer2);

    auto addscorecommand = std::make_shared<AddScoreCommand>();
    addscorecommand->SetActor(player2);
    Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_E, SDL_KEYDOWN), addscorecommand);

    auto textObjectLivesPlayer2Component = textObjectLivesPlayer2->GetComponent<TextObject>();
    auto weakHealthComponentPlayer2 = player2->GetComponent<HealthComponent>();


    Input::GetInstance().AddController(std::make_shared<GamePad>(0));

    moveCommand = std::make_shared<MoveCommand>(player2, glm::vec3{ 0.f, -1.f, 0.f }, moveSpeed * 2.f);
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::DPadUp, KeyState::Pressed), moveCommand);

    moveCommand = std::make_shared<MoveCommand>(player2, glm::vec3{ 0.f, 1.f, 0.f }, moveSpeed * 2.f);
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::DPadDown, KeyState::Pressed), moveCommand);

    moveCommand = std::make_shared<MoveCommand>(player2, glm::vec3{ -1.f, 0.f, 0.f }, moveSpeed * 2.f);
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::DPadLeft, KeyState::Pressed), moveCommand);

    moveCommand = std::make_shared<MoveCommand>(player2, glm::vec3{ 1.f, 0.f, 0.f }, moveSpeed * 2.f);
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::DPadRight, KeyState::Pressed), moveCommand);

    scene.Add(player2);
}

int main(int, char* []) {
    std::string dataPath = "../Data/";
    if (fs::exists(dataPath) and fs::is_directory(dataPath)) {}
    else {
        dataPath = "../../Data/";
    }
    try
    {
        dae::Minigin engine(dataPath);
        engine.Run(loadgame);
    }
    catch (...)
    {
        LOG_CRITICAL("");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}