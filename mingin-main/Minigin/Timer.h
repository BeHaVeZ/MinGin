#pragma once

#include <chrono>
#include "Singleton.h"

namespace dae
{
	class Timer final: public Singleton<Timer>
	{
	public:
		void Start();
		void Update();

		float GetDeltaTime() const;
		int GetFPS() const;

	private:
		friend class Singleton<Timer>;
		Timer();
		std::chrono::high_resolution_clock::time_point m_LastTime;
		float m_DeltaTime;
		int m_FrameCount;
		int m_FPS;
	};
}
