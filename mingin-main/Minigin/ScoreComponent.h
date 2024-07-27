#pragma once
#include "Component.h"
#include "Subject.h"
#include "Observer.h"

namespace dae
{
    class ScoreComponent final : public Component, public Subject
    {
    public:
        ScoreComponent() = default;
        ~ScoreComponent() = default;

        void Update() override {}
        void Render() const override {}

        int GetScore() const { return m_Score; }

        void SetScore(int score)
        {
            m_Score = score;
            Notify(Event::PointsIncrease);
        }

        void AddScore(int score)
        {
            m_Score += score;
            Notify(Event::PointsIncrease);
        }

    private:
        int m_Score{ 0 };
    };
}
