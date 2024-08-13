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
#include "Prototype.h"
#include "SlickSam.h"
#include "SoundSystem.h"
#include "ServiceLocator.h"

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

enum class Sounds : unsigned short
{
	swear = 0,
	jump,
	jump2,
	die,
	intro,
	levelComplete
};


const int g_CubesSpriteWidth = 32;
const int g_CubesSpriteHeight = 32;
std::vector<std::shared_ptr<dae::GameObject>> g_QBertGOs;


void SetUpGlobalGameObjects();
void LoadSoundSystem();
void LoadFirstLvl();
void LoadSecondLvl();
void LoadThirdLvl();

void loadgame()
{
	LoadSoundSystem();
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
	g_QBertGOs = MakeQBert();

	auto bindCommand = [](const std::shared_ptr<GameObject>& qBertGO, SDL_Scancode key, std::shared_ptr<Command> command) {
		command->SetActor(qBertGO);
		Input::GetInstance().AddCommand(std::make_pair(key, SDL_KEYDOWN), command);
		};

	bindCommand(g_QBertGOs[0], SDL_SCANCODE_W, std::make_shared<QBertMoveUpCommand>());
	bindCommand(g_QBertGOs[0], SDL_SCANCODE_S, std::make_shared<QBertMoveDownCommand>());
	bindCommand(g_QBertGOs[0], SDL_SCANCODE_A, std::make_shared<QBertMoveLeftCommand>());
	bindCommand(g_QBertGOs[0], SDL_SCANCODE_D, std::make_shared<QBertMoveRightCommand>());
	bindCommand(g_QBertGOs[0], SDL_SCANCODE_F1, std::make_shared<SkipLevelCommand>());
	bindCommand(g_QBertGOs[0], SDL_SCANCODE_M, std::make_shared<MuteSoundCommand>());



	Input::GetInstance().AddController(std::make_shared<GamePad>(0));

	auto bindControllerCommand = [](const std::shared_ptr<GameObject>& qBertGO, GamePad::ControllerButton button, std::shared_ptr<Command> command) {
		command->SetActor(qBertGO);
		Input::GetInstance().AddCommand(std::make_tuple(0, button, KeyState::Down), command);
		};

	bindControllerCommand(g_QBertGOs[0], GamePad::ControllerButton::DPadUp, std::make_shared<QBertMoveUpCommand>());
	bindControllerCommand(g_QBertGOs[0], GamePad::ControllerButton::DPadDown, std::make_shared<QBertMoveDownCommand>());
	bindControllerCommand(g_QBertGOs[0], GamePad::ControllerButton::DPadLeft, std::make_shared<QBertMoveLeftCommand>());
	bindControllerCommand(g_QBertGOs[0], GamePad::ControllerButton::DPadRight, std::make_shared<QBertMoveRightCommand>());
}

void LoadSoundSystem()
{
#if _DEBUG
	ServiceLocator::RegisterSoundSystem(new Logging_SoundSystem(new SDL_SoundSystem()));
#else
	ServiceLocator::RegisterSoundSystem(new SDL_SoundSystem());
#endif
	ServiceLocator::GetSoundSystem().AddSound("swear.wav", (unsigned short)Sounds::swear, false);
	ServiceLocator::GetSoundSystem().AddSound("jump.wav", (unsigned short)Sounds::jump, false);
	ServiceLocator::GetSoundSystem().AddSound("jump-2.wav", (unsigned short)Sounds::jump2, false);
	ServiceLocator::GetSoundSystem().AddSound("jump-3.wav", (unsigned short)Sounds::die, false);
	ServiceLocator::GetSoundSystem().AddSound("tune.wav", (unsigned short)Sounds::intro, false);
	ServiceLocator::GetSoundSystem().AddSound("tune-2.wav", (unsigned short)Sounds::levelComplete, false);

	ServiceLocator::GetSoundSystem().PlaySound((unsigned short)Sounds::intro, 50.f);
}

void LoadFirstLvl()
{
	auto& levelTitleScene = SceneManager::GetInstance().CreateScene("Level0A");

	levelTitleScene.Add(MakeLevelTitle(1));
	levelTitleScene.Add(MakeLevelTransition());

	levelTitleScene.Init();
	

	auto& scene1 = SceneManager::GetInstance().CreateScene("Level1A");
	auto pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight,
		0, 1, g_CubesSpriteWidth, g_CubesSpriteHeight);
	for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
		scene1.Add(cube);

	for (auto& go : g_QBertGOs)
	{
		scene1.Add(go);
	}

	auto levelObserver = std::make_shared<GameObject>();
	levelObserver->AddComponent<LevelObserver>(levelObserver, g_QBertGOs[0]->GetComponent<QBertCharacter>(),
		std::move(pyramid), 1, false, false);

	scene1.Add(levelObserver);


	auto& scene2 = SceneManager::GetInstance().CreateScene("Level1B");
	pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight,
		1, 1, g_CubesSpriteWidth, g_CubesSpriteHeight);
	for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
		scene2.Add(cube);
	for (auto& go : g_QBertGOs)
	{
		scene2.Add(go);
	}

	levelObserver = std::make_shared<GameObject>();
	levelObserver->AddComponent<LevelObserver>(levelObserver, g_QBertGOs[0]->GetComponent<QBertCharacter>(),
		std::move(pyramid), 1, false, false);
	scene2.Add(levelObserver);


	auto& scene3 = SceneManager::GetInstance().CreateScene("Level1C");
	pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight,
		2, 1, g_CubesSpriteWidth, g_CubesSpriteHeight);
	for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
		scene3.Add(cube);
	for (auto& go : g_QBertGOs)
	{
		scene3.Add(go);
	}

	levelObserver = std::make_shared<GameObject>();
	levelObserver->AddComponent<LevelObserver>(levelObserver, g_QBertGOs[0]->GetComponent<QBertCharacter>(),
		std::move(pyramid), 1, false, true, 0, 0, 10.f, 1.f);
	scene3.Add(levelObserver);



	auto& scene4 = SceneManager::GetInstance().CreateScene("Level01D");
	pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight,
		3, 1, g_CubesSpriteWidth, g_CubesSpriteHeight);
	for (const std::shared_ptr<dae::GameObject>& cube : pyramid->m_CubeGOVector)
		scene4.Add(cube);
	for (auto& go : g_QBertGOs)
	{
		scene4.Add(go);
	}

	levelObserver = std::make_shared<GameObject>();
	levelObserver->AddComponent<LevelObserver>(levelObserver, g_QBertGOs[0]->GetComponent<QBertCharacter>(),
		std::move(pyramid), 1, false, true, 0, 0, 7.f, 1.f);
	scene4.Add(levelObserver);

}


void LoadSecondLvl()
{
	auto& levelTitleScene = SceneManager::GetInstance().CreateScene("Level0B");

	levelTitleScene.Add(MakeLevelTitle(2));
	levelTitleScene.Add(MakeLevelTransition());

	auto& scene1 = SceneManager::GetInstance().CreateScene("Level2A");
	auto pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight,
		1, 2, g_CubesSpriteWidth, g_CubesSpriteHeight);
	for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
		scene1.Add(cube);

	for (auto& go : g_QBertGOs)
	{
		scene1.Add(go);
	}
	auto levelObserver = std::make_shared<GameObject>();
	levelObserver->AddComponent<LevelObserver>(levelObserver, g_QBertGOs[0]->GetComponent<QBertCharacter>(),
		std::move(pyramid), 2, true, true, 20.f, 1.f, 15.f, 1.f);
	scene1.Add(levelObserver);

	auto& scene2 = SceneManager::GetInstance().CreateScene("Level2B");
	pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight,
		3, 2, g_CubesSpriteWidth, g_CubesSpriteHeight);
	for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
		scene2.Add(cube);
	for (auto& go : g_QBertGOs)
	{
		scene2.Add(go);
	}
	levelObserver = std::make_shared<GameObject>();
	levelObserver->AddComponent<LevelObserver>(levelObserver, g_QBertGOs[0]->GetComponent<QBertCharacter>(),
		std::move(pyramid), 2, true, true, 15.f, 1.f, 10.f, 1.f);
	scene2.Add(levelObserver);



	auto& scene3 = SceneManager::GetInstance().CreateScene("Level2C");

	pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight,
		0, 2, g_CubesSpriteWidth, g_CubesSpriteHeight);
	for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
		scene3.Add(cube);

	for (auto& go : g_QBertGOs)
	{
		scene3.Add(go);
	}
	levelObserver = std::make_shared<GameObject>();
	levelObserver->AddComponent<LevelObserver>(levelObserver, g_QBertGOs[0]->GetComponent<QBertCharacter>(),
		std::move(pyramid), 2, true, true, 10.f, 1.f, 7.f, 1.f);
	scene3.Add(levelObserver);

	auto& scene4 = SceneManager::GetInstance().CreateScene("Level2D");

	pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight,
		4, 2, g_CubesSpriteWidth, g_CubesSpriteHeight);
	for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
		scene4.Add(cube);

	for (auto& go : g_QBertGOs)
	{
		scene4.Add(go);
	}

	levelObserver = std::make_shared<GameObject>();
	levelObserver->AddComponent<LevelObserver>(levelObserver, g_QBertGOs[0]->GetComponent<QBertCharacter>(),
		std::move(pyramid), 2, true, true, 5.f, 1.f, 5.f, 1.f);
	scene4.Add(levelObserver);


}

void LoadThirdLvl()
{
	auto& levelTitleScene = SceneManager::GetInstance().CreateScene("Level0C");

	levelTitleScene.Add(MakeLevelTitle(3));
	levelTitleScene.Add(MakeLevelTransition());

	auto& scene1 = SceneManager::GetInstance().CreateScene("Level3A");
	auto pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight,
		5, 3, g_CubesSpriteWidth, g_CubesSpriteHeight);
	for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
		scene1.Add(cube);

	for (auto& go : g_QBertGOs)
	{
		scene1.Add(go);
	}
	auto levelObserver = std::make_shared<GameObject>();
	levelObserver->AddComponent<LevelObserver>(levelObserver, g_QBertGOs[0]->GetComponent<QBertCharacter>(),
		std::move(pyramid), 3, true, true, 25.f, 1.f, 15.f, 1.f);
	scene1.Add(levelObserver);


	auto& scene2 = SceneManager::GetInstance().CreateScene("Level2B");
	pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight,
		2, 3, g_CubesSpriteWidth, g_CubesSpriteHeight);
	for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
		scene2.Add(cube);

	for (auto& go : g_QBertGOs)
	{
		scene2.Add(go);
	}
	levelObserver = std::make_shared<GameObject>();
	levelObserver->AddComponent<LevelObserver>(levelObserver, g_QBertGOs[0]->GetComponent<QBertCharacter>(),
		std::move(pyramid), 3, true, true, 20.f, 1.f, 10.f, 1.f);
	scene2.Add(levelObserver);

	auto& scene3 = SceneManager::GetInstance().CreateScene("Level2C");

	pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight,
		1, 3, g_CubesSpriteWidth, g_CubesSpriteHeight);
	for (const std::shared_ptr<GameObject>& cube : pyramid->m_CubeGOVector)
		scene3.Add(cube);

	for (auto& go : g_QBertGOs)
	{
		scene3.Add(go);
	}

	levelObserver = std::make_shared<GameObject>();
	levelObserver->AddComponent<LevelObserver>(levelObserver, g_QBertGOs[0]->GetComponent<QBertCharacter>(),
		std::move(pyramid), 3, true, true, 15.f, 1.f, 7.f, 1.f);
	scene3.Add(levelObserver);

	auto& scene4 = dae::SceneManager::GetInstance().CreateScene("Level2D");

	pyramid = std::make_unique<Pyramid>(300.f, 80.f, g_NrRows, g_CubesActualWidth, g_CubesActualHeight,
		0, 3, g_CubesSpriteWidth, g_CubesSpriteHeight);
	for (const std::shared_ptr<dae::GameObject>& cube : pyramid->m_CubeGOVector)
		scene4.Add(cube);

	for (auto& go : g_QBertGOs)
	{
		scene4.Add(go);
	}

	levelObserver = std::make_shared<GameObject>();
	levelObserver->AddComponent<LevelObserver>(levelObserver, g_QBertGOs[0]->GetComponent<QBertCharacter>(),
		std::move(pyramid), 3, true, true, 10.f, 1.f, 5.f, 1.f);
	scene4.Add(levelObserver);

	auto& victoryScene = dae::SceneManager::GetInstance().CreateScene("VictoryScene");
	victoryScene.Add(MakeWinScreen());
}
