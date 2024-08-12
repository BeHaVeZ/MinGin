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
		QBertMoved,
		SlickSamMoved,
		SlickSamFell,
		UggWrongwayMoved,
		UggWrongwayFell,
		//This will be used for anims (extra)
		JumpUpRight,
		JumpUpLeft,
		JumpDownRight,
		JumpDownLeft,
		JumpLeft,
		JumpRight
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void OnNotify(const Event& event) = 0;
	};
}

