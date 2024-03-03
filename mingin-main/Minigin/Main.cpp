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



    auto bomb = CREATE_GAMEOBJECT();
    bomb->AddComponent<TextureComponent>("bomb.png");
    bomb->AddComponent<dae::RotationComponent>(20.f, 5.f,true);

    auto bomberman = CREATE_GAMEOBJECT();
    bomberman->AddComponent<TextureComponent>("bomberman.png");
    bomberman->AddComponent<RotationComponent>(20.f, 5.f,false);
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
        LOG_CRITICAL("Ja kijk maar nog is na waar je data folder staat.");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}