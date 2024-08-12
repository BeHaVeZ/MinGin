#pragma once
#include <memory>
#include <vector>
#include "Observer.h"
#include "Component.h"
#include "SlickSam.h"
#include "UggWrongway.h"

namespace dae
{
	class Pyramid;
	class QBertCharacter;

	class LevelObserver final : public Component, public Observer
	{
	public:
		explicit LevelObserver(float transitionTime);
		explicit LevelObserver(const std::shared_ptr<GameObject>& gameObject, const std::shared_ptr<QBertCharacter>& qBertComp, std::unique_ptr<Pyramid> pyramid,int level,
			bool spawnSlickSams,
			bool spawnUggWrongways,
			float slickSamSpawnInterval = 0,
			float slickSamMoveInterval = 0,
			float uggWrongSpawnInterval = 0,
			float uggWrongMoveInterval = 0);
		~LevelObserver() override;
		/*****************************************************************************/
		void SetQBert(const std::shared_ptr<QBertCharacter>& qBertComp);
		void SetPyramid(std::unique_ptr<Pyramid> pyramid);
		/*****************************************************************************/
		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() const override {};
		void OnNotify(const Event& event) override;
		/*****************************************************************************/
		bool CheckAllCubesTurned() const;
		void AddSlickSam(bool isSlick, bool isLeft);
		void AddUggWrongway(bool isUgg, bool isLeft);
		/*****************************************************************************/
		void WinLevel();
		void LevelWonAnimation();
		void ChangeFreezeEverything(bool freeze);
		void ChangeLevel() const;
		void ClearLevel();
		bool CheckCollidingUggWrong() const;
		void KillCollidingSlickSam();
		void KillFallenSlickSam();
		void KillFallenUggWrongway();


	private:
		std::weak_ptr<QBertCharacter> m_QBertComp;
		std::unique_ptr<Pyramid> m_Pyramid;
		std::weak_ptr<GameObject> m_GameObject;
		bool m_LevelComplete;
		float m_AnimationTimer, m_FullAnimationTime;
		float m_FlashingTimer, m_FlashingColorTime;
		float m_PostAnimationTimer, m_PostAnimationPause;
		int m_CurrentFlashingColor;
		bool m_EverythingClear;
		const int m_Level;
		/*****************************************************************************/
		std::vector<std::shared_ptr<UggWrongWay>> m_UggWrongCompVector;
		const bool m_SpawnUggWrongways;
		float m_UggWrongwaySpawnTimer1;
		float m_UggWrongwaySpawnTimer2;
		float m_UggWrongSpawnInterval;
		float m_UggWrongMoveInterval;
		/*****************************************************************************/
		std::vector<std::shared_ptr<SlickSam>> m_SlickSamCompVector;
		const bool m_SpawnSlickSams;
		float m_SlickSamSpawnTimer;
		float m_SlickSamSpawnInterval;
		float m_SlickSamMoveInterval;
		bool m_SlickSamVectorModified = false;
		/*****************************************************************************/
		float m_LevelTitleTimer, m_LevelTitleScreenTime;
		/*****************************************************************************/
		bool m_IsDeadQbert;
		float m_DeadQbertTimer, m_DeadQbertMaxTime;
		bool m_DeathEmptyScene;
		float m_DeathEmptySceneTimer, m_DeathEmptySceneMaxTime;
	};
}
