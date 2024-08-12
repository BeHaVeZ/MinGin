#include "pch.h"


namespace dae
{

	void SceneManager::Update()
	{
		m_scenes[m_CurrentScene]->Update();
	}

	void SceneManager::Render()
	{
		m_scenes[m_CurrentScene]->Render();
	}

	void SceneManager::ChangeScene(int newSceneIdx)
	{
		if (newSceneIdx < int(m_scenes.size()))
		{
			m_CurrentScene = newSceneIdx;
			m_scenes[m_CurrentScene]->Init();
		}
		else
			std::cout << "Scene failed to load check indexes\n";
	}

	Scene& SceneManager::CreateScene(const std::string& name)
	{
		const auto& scene = std::shared_ptr<Scene>(new Scene(name));
		m_scenes.push_back(scene);
		return *scene;
	}

}
