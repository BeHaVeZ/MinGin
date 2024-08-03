#pragma once
#include <memory>
#include "Component.h"
#include "Observer.h"
#include <vector>

namespace dae
{
	class LevelObserver;

	class GameObserver : public Component, public Observer
	{
	public:
		explicit GameObserver(const std::shared_ptr<GameObject>& gameObject, LevelObserver* lvlObserverComponent, std::vector<std::shared_ptr<GameObject>>* nextGameObservers);
		~GameObserver() override;


		void SetLvlObserver(const std::shared_ptr<LevelObserver>& lvlObserverComponent);

		void Initialize() override;
		void Update() override {}
		void OnNotify(const Event& event) override;

		void WinLevel();
		void WinGame();


	private:
		std::shared_ptr<LevelObserver> m_LvlObserverComponent;
		std::vector<std::shared_ptr<GameObject>>* m_NextGameObservers{};
		std::shared_ptr<GameObject> m_GameObject{};
		int m_CurrentLevel;
		int m_CurrentLevelCounter;
	};
}