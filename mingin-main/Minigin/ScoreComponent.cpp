#include "pch.h"

dae::ScoreComponent::ScoreComponent()
{
	m_pOnScoreChanged = std::make_unique<Event<int>>();
}
