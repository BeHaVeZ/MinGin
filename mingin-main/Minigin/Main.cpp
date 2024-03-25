#include "pch.h"


#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif


namespace fs = std::filesystem;
//TODO: TOBEDELETED {R}
#pragma region R
enum Achievements
{
    ACH_WIN_ONE_GAME
};

dae::Achievement_t g_Achievements[] = { _ACH_ID(ACH_WIN_ONE_GAME,"Winner") };
std::unique_ptr<dae::CSteamAchievements> g_SteamAchievements{ nullptr };

#pragma endregion

void load()
{
    SteamUserStats()->ResetAllStats(true);
    SteamUserStats()->StoreStats();
    using namespace dae;
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = CREATE_GAMEOBJECT();
	go->AddComponent<TextureComponent>("background.tga");
	scene.Add(go);

	go = CREATE_GAMEOBJECT();
	go->AddComponent<TextureComponent>("logo.tga");
	go->SetPosition(216, 180);
	scene.Add(go);

	auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 25);
	auto to = CREATE_GAMEOBJECT();
	to->AddComponent<TextObject>("Programming 4 Assignment", font);
	to->GetComponent<TextObject>()->SetPosition(20, 20);
	to->SetPosition(60, 20);
	scene.Add(to);

    auto co = CREATE_GAMEOBJECT();
    co->AddComponent<TextObject>("Move(WASD/DPAD), add score(R/X), Get damage(E/A)", font);
    co->SetPosition(20, 400);
    scene.Add(co);

	auto fpsObject = CREATE_GAMEOBJECT();
	fpsObject->AddComponent<FPSCounter>(font);
    scene.Add(fpsObject);


    auto player1 = CREATE_GAMEOBJECT();
    player1->AddComponent<TextureComponent>("bomb.png");
    player1->AddComponent<HealthComponent>(3);
    player1->AddComponent<ScoreComponent>();
    player1->SetPosition(100.f, 100.f);

    auto textObjectLives = CREATE_GAMEOBJECT();
    textObjectLives->AddComponent<TextObject>("Lives player 1: 3", font);
    textObjectLives->SetPosition(50.f, 300.f);
    scene.Add(textObjectLives);

    auto textObjectLivesComponent = textObjectLives->GetComponent<TextObject>();
    auto weakHealthComponent = player1->GetComponent<HealthComponent>();

    weakHealthComponent->GetOnDeathEvent()->AddListener([textObjectLivesComponent](int lives) {
        textObjectLivesComponent->SetText("Lives player 1: " + std::to_string(lives));
        });


    auto textObjectScore = CREATE_GAMEOBJECT();
    textObjectScore->AddComponent<TextObject>("Score: 0", font);
    textObjectScore->SetPosition(50.f, 350.f);
    scene.Add(textObjectScore);

    auto textObjectScoreComponent = textObjectScore->GetComponent<TextObject>();
    auto weakScoreComponent = player1->GetComponent<ScoreComponent>();

    weakScoreComponent->GetOnScoreChangedEvent()->AddListener([textObjectScoreComponent](int score) {
        textObjectScoreComponent->SetText("Score: " + std::to_string(score));
        if (score >= 500)
        {
            g_SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
        }
        });


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

    auto killCommand = std::make_shared<KillCommand>(player1->GetComponent<HealthComponent>());
    Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_E, SDL_KEYDOWN), killCommand);
    auto addScoreCommand = std::make_shared<AddScoreCommand>(player1->GetComponent<ScoreComponent>());
    Input::GetInstance().AddCommand(std::make_pair(SDL_SCANCODE_R, SDL_KEYDOWN), addScoreCommand);


    auto player2 = CREATE_GAMEOBJECT();
    player2->AddComponent<TextureComponent>("bomberman.png");
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

    auto textObjectLivesPlayer2Component = textObjectLivesPlayer2->GetComponent<TextObject>();
    auto weakHealthComponentPlayer2 = player2->GetComponent<HealthComponent>();
    weakHealthComponentPlayer2->GetOnDeathEvent()->AddListener([textObjectLivesPlayer2Component](int lives) {
        textObjectLivesPlayer2Component->SetText("Lives player 2: " + std::to_string(lives));
        });

    auto textObjectScorePlayer2Component = textObjectScorePlayer2->GetComponent<TextObject>();
    auto weakScoreComponentPlayer2 = player2->GetComponent<ScoreComponent>();
    weakScoreComponentPlayer2->GetOnScoreChangedEvent()->AddListener([textObjectScorePlayer2Component](int score) {
        textObjectScorePlayer2Component->SetText("Score: " + std::to_string(score));
        if (score >= 500)
        {
            g_SteamAchievements->SetAchievement("ACH_WIN_ONE_GAME");
        }
        });


    Input::GetInstance().AddController(std::make_shared<GamePad>(0));

    moveCommand = std::make_shared<MoveCommand>(player2, glm::vec3{ 0.f, -1.f, 0.f }, moveSpeed * 2.f);
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::DPadUp, KeyState::Pressed), moveCommand);

    moveCommand = std::make_shared<MoveCommand>(player2, glm::vec3{ 0.f, 1.f, 0.f }, moveSpeed * 2.f);
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::DPadDown, KeyState::Pressed), moveCommand);

    moveCommand = std::make_shared<MoveCommand>(player2, glm::vec3{ -1.f, 0.f, 0.f }, moveSpeed * 2.f);
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::DPadLeft, KeyState::Pressed), moveCommand);

    moveCommand = std::make_shared<MoveCommand>(player2, glm::vec3{ 1.f, 0.f, 0.f }, moveSpeed * 2.f);
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::DPadRight, KeyState::Pressed), moveCommand);

    killCommand = std::make_shared<KillCommand>(player2->GetComponent<HealthComponent>());
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::ButtonSouth, KeyState::Down), killCommand);

    addScoreCommand = std::make_shared<AddScoreCommand>(player2->GetComponent<ScoreComponent>());
    Input::GetInstance().AddCommand(std::make_tuple(0, GamePad::ControllerButton::ButtonWest, KeyState::Down), addScoreCommand);

    scene.Add(player2);
}

int main(int, char* []) {

    if (!SteamAPI_Init())
    {
        std::cerr << "Fatal Error - Steam must be running to play this game (SteamAPI_Init() failed)." << std::endl;
        return 1;
    }
    else
    {
        std::cout << "Successfully initialized steam." << std::endl;
        g_SteamAchievements = std::make_unique<dae::CSteamAchievements>(g_Achievements, 1);
    }

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
    SteamAPI_Shutdown();
    return EXIT_SUCCESS;
}