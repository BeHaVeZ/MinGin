#include "pch.h"


#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif


namespace fs = std::filesystem;

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



    auto player1 = CREATE_GAMEOBJECT();
    player1->AddComponent<TextureComponent>("bomb.png");
    player1->SetPosition(100.f, 100.f);

    auto player2 = CREATE_GAMEOBJECT();
    player2->AddComponent<TextureComponent>("bomberman.png");
    player2->SetPosition(200.f, 200.f);

	scene.Add(fpsObject);
	scene.Add(player2);
	scene.Add(player1);

    const float moveSpeed{ 20.f };

    Input::GetInstance().AddController(std::make_shared<GamePad>(0));

    auto moveCommand = std::make_shared<MoveCommand>(player2, glm::lowp_vec3{ 0.f,-1.f,0.f }, moveSpeed);
    Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_W, SDL_KEYDOWN), moveCommand);

    moveCommand = std::make_shared<MoveCommand>(player2, glm::vec3{ -1.f, 0.f, 0.f }, moveSpeed);
    Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_A, SDL_KEYDOWN), moveCommand);

    moveCommand = std::make_shared<MoveCommand>(player2, glm::vec3{ 0.f, 1.f, 0.f }, moveSpeed);
    Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_S, SDL_KEYDOWN), moveCommand);

    moveCommand = std::make_shared<MoveCommand>(player2, glm::vec3{ 1.f, 0.f, 0.f }, moveSpeed);
    Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_D, SDL_KEYDOWN), moveCommand);


    moveCommand = std::make_shared<MoveCommand>(player1, glm::vec3{ 0.f, -1.f, 0.f }, moveSpeed * 2.f);
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::DPadUp, KeyState::Pressed), moveCommand);

    moveCommand = std::make_shared<MoveCommand>(player1, glm::vec3{ 0.f, 1.f, 0.f }, moveSpeed * 2.f);
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::DPadDown, KeyState::Pressed), moveCommand);

    moveCommand = std::make_shared<MoveCommand>(player1, glm::vec3{ -1.f, 0.f, 0.f }, moveSpeed * 2.f);
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::DPadLeft, KeyState::Pressed), moveCommand);

    moveCommand = std::make_shared<MoveCommand>(player1, glm::vec3{ 1.f, 0.f, 0.f }, moveSpeed * 2.f);
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::DPadRight, KeyState::Pressed), moveCommand);
}

int main(int, char* []) {
    std::string dataPath = "../Data/";
	if (fs::exists(dataPath) and fs::is_directory(dataPath)){}
    else {
        dataPath = "../../Data/";
    }
    try
    {
        dae::Minigin engine(dataPath);
        engine.Run(load);
    }
    catch (...)
    {
        LOG_CRITICAL("Ja kijk maar nog is na waar je data folder staat.");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}