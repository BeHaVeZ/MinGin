#pragma once

namespace dae
{
	class TextComponent;
	class GameObject;

	enum class Event
	{
		ActorHealthChange,
		ActorDeath,
		PointsIncrease,
		TileChange,
		ColorChange,
		CoilyDeath,
		SlickSamCaught,
		QBertMove,
		SlickSamMove,
		SlickSamFell
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(const Event& event) = 0;
	};
}

