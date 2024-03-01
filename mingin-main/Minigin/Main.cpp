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

namespace fs = std::filesystem;

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>("background.tga");
	scene.Add(go);

	go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::TextureComponent>("logo.tga");
	go->SetPosition(216, 180);
	scene.Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_shared<dae::GameObject>();
	to->AddComponent<dae::TextObject>("Programming 4 Assignment", font);
	to->GetComponent<dae::TextObject>()->SetPosition(20, 20);
	to->SetPosition(60, 20);
	scene.Add(to);

	auto fpsObject = std::make_shared<dae::GameObject>();
	fpsObject->AddComponent<dae::FPSCounter>(font);


	auto bomberman = std::make_shared<dae::GameObject>();
	bomberman->AddComponent<dae::TextureComponent>("bomberman.png");


	bomberman->GetTransform().SetPosition(120, 120, 0);

	scene.Add(fpsObject);
	scene.Add(bomberman);
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