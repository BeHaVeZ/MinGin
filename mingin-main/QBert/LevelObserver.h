#pragma once
#include <memory>
#include <vector>
#include "Observer.h"
#include "Component.h"
#include "SlickSam.h"

namespace dae
{
	class Pyramid;
	class QBertCharacter;

	class LevelObserver final : public Component, public Observer
	{
	public:
		explicit LevelObserver(const std::shared_ptr<GameObject>& gameObject, const std::shared_ptr<QBertCharacter>& qBertComp, std::unique_ptr<Pyramid> pyramid,bool spawnSlickSams = false,float slickSamSpawnInterval = 0,float slickSamMoveInterval = 0);
		~LevelObserver() override;

		void SetQBert(const std::shared_ptr<QBertCharacter>& qBertComp);
		void SetPyramid(std::unique_ptr<Pyramid> pyramid);
		void SetSlickSamVector(std::vector<std::shared_ptr<SlickSam>> slickSamCompVector);

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() const override {};
		void OnNotify(const Event& event) override;

		bool CheckAllCubesTurned() const;
		void AddSlickSam(bool isSlick, bool isLeft);

		void WinLevel();



	private:
		std::weak_ptr<QBertCharacter> m_QBertComp;
		std::vector<std::shared_ptr<SlickSam>> m_SlickSamCompVector;
		std::unique_ptr<Pyramid> m_Pyramid;
		std::weak_ptr<GameObject> m_GameObject;
		bool m_LevelComplete;

		const bool m_SpawnSlickSams;
		float m_SlickSamSpawnTimer;
		float m_SlickSamSpawnInterval;
		float m_SlickSamMoveInterval;
		bool m_SlickSamVectorModified = false;
	};
}
