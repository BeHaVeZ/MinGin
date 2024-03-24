#pragma once

namespace dae
{

	class ScoreComponent final : public Component
	{
	public:
		ScoreComponent();
		~ScoreComponent() = default;


		virtual void Update() override {};
		virtual void Render() const override {};

		int GetScore() const { return m_Score; }
		Event<int>* GetOnScoreChangedEvent() const { return m_pOnScoreChanged.get(); }


		void SetScore(int score) { m_Score = score;  m_pOnScoreChanged->Invoke(m_Score); }
		void AddScore(int score) { m_Score += score; m_pOnScoreChanged->Invoke(m_Score); }


	private:
		int m_Score{ 0 };
		std::unique_ptr<Event<int>> m_pOnScoreChanged;
	};
}

