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

	scene.Add(fpsObject);
}

int main(int, char* []) {
	try
	{
		dae::Minigin engine("../Data/");
		engine.Run(load);
	}
	catch (...)
	{
		dae::Minigin engine("../../Data/");
		engine.Run(load);
	}
	return 0;
}