#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObject.h"
#include "Scene.h"
#include "Component.h"
#include "GameObject.h"
#include "TextObject.h"
#include "TextureComponent.h"
#include "FPSCounter.h"
#include <filesystem>
#include <iostream>
#include "RotationComponent.h"

#define CREATE_GAMEOBJECT(...) std::make_shared<dae::GameObject>(__VA_ARGS__)
namespace fs = std::filesystem;

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = CREATE_GAMEOBJECT();
	go->AddComponent<dae::TextureComponent>("background.tga");
	scene.Add(go);

	go = CREATE_GAMEOBJECT();
	go->AddComponent<dae::TextureComponent>("logo.tga");
	go->SetPosition(216, 180);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = CREATE_GAMEOBJECT();
	to->AddComponent<dae::TextObject>("Programming 4 Assignment", font);
	to->GetComponent<dae::TextObject>()->SetPosition(20, 20);
	to->SetPosition(60, 20);
	scene.Add(to);

	auto fpsObject = CREATE_GAMEOBJECT();
	fpsObject->AddComponent<dae::FPSCounter>(font);



    auto bomb = CREATE_GAMEOBJECT();
    bomb->AddComponent<dae::TextureComponent>("bomb.png");
    bomb->AddComponent<dae::RotationComponent>(20.f, 2.f);

    auto bomberman = CREATE_GAMEOBJECT();
    bomberman->AddComponent<dae::TextureComponent>("bomberman.png");
    bomberman->AddComponent<dae::RotationComponent>(20.f, 2.f);
    bomberman->AddChild(bomb);

    auto centerPoint = CREATE_GAMEOBJECT();
    centerPoint->SetPosition(100, 300);
    centerPoint->AddChild(bomberman);

	scene.Add(fpsObject);
    scene.Add(centerPoint);
	scene.Add(bomberman);
	scene.Add(bomb);
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
        std::cerr << "Ja kijk maar nog is na waar je data folder staat.\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}