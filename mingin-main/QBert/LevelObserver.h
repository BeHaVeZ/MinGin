#pragma once
#include <memory>
#include "Observer.h"
#include "Component.h"

namespace dae
{
	class Pyramid;
	class QBertCharacter;

	class LevelObserver final : public Component, public Observer
	{
	public:
		explicit LevelObserver(const std::shared_ptr<GameObject>& gameObject, const std::shared_ptr<QBertCharacter>& qBertComp, std::unique_ptr<Pyramid> pyramid);
		~LevelObserver() override;

		void SetQBert(const std::shared_ptr<QBertCharacter>& qBertComp);
		void SetPyramid(std::unique_ptr<Pyramid> pyramid);

		virtual void Initialize() override;
		virtual void Update() override {}
		virtual void Render() const override {};
		void OnNotify(const Event& event) override;

		bool CheckAllCubesTurned() const;
		void WinLevel();

	private:
		std::weak_ptr<QBertCharacter> m_QBertComp;
		std::unique_ptr<Pyramid> m_Pyramid;
		std::weak_ptr<GameObject> m_GameObject;
		bool m_LevelComplete;
	};
}
