#include "GameObserver.h"

#include <iostream>
#include "GameObject.h"
#include "LevelObserver.h"
#include "SceneManager.h"
#include "Subject.h"

namespace dae
{



	GameObserver::GameObserver(const std::shared_ptr<dae::GameObject>& gameObject, LevelObserver* lvlOBserverComponent, std::vector<std::shared_ptr<dae::GameObject>>* nextGameObservers)
		: m_LvlObserverComponent(lvlOBserverComponent)
		, m_NextGameObservers(nextGameObservers)
		, m_GameObject(gameObject)
		, m_CurrentLevel(1)
		, m_CurrentLevelCounter(1)
	{}

	GameObserver::~GameObserver()
	{
		if (m_LvlObserverComponent != nullptr)
			m_LvlObserverComponent->GetSubject()->RemoveObserver(this);
	}

	void GameObserver::Initialize()
	{
		if (m_LvlObserverComponent != nullptr)
			m_LvlObserverComponent->GetSubject()->AddObserver(this);
	}

	void GameObserver::SetLvlObserver(const std::shared_ptr<LevelObserver>& lvlGameObserverComponent)
	{
		if (m_LvlObserverComponent != nullptr)
			m_LvlObserverComponent->GetSubject()->RemoveObserver(this);

		m_LvlObserverComponent = lvlGameObserverComponent;

		if (m_LvlObserverComponent != nullptr)
			m_LvlObserverComponent->GetSubject()->AddObserver(this);
	}

	void GameObserver::OnNotify(const Event& event)
	{
		switch (event)
		{
		case dae::Event::ColorChange:
			m_CurrentLevel++;
			m_CurrentLevelCounter++;
			if (m_CurrentLevel > 4)
			{
				m_CurrentLevel = 1;
				WinLevel();
			}
			if (m_CurrentLevelCounter <= m_NextGameObservers->size())
			{
				auto& scene = dae::SceneManager::GetInstance();
				scene.ChangeScene(scene.GetCurrentSceneIdx() + 1);
				SetLvlObserver(m_NextGameObservers->operator[](m_CurrentLevelCounter)->GetComponent<LevelObserver>());
			}
			else
			{
				WinGame();
			}
			break;
		}
	}

	void GameObserver::WinLevel()
	{
		std::cout << "level finished\n";
	}

	void GameObserver::WinGame()
	{
		std::cout << "game finished\n";
	}
}