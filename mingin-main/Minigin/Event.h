#pragma once

namespace dae
{
	template<typename... Args>
	class Event
	{
	public:
		using Callback = std::function<void(Args...)>;
		Event() = default;
		~Event() = default;

		void AddListener(const Callback& callback)
		{
			m_Listeners.push_back(callback);
		}

		void RemoveListener(const Callback& callback)
		{
			for (auto iterator = m_Listeners.begin(); iterator != m_Listeners.end(); ++iterator)
			{
				if (*iterator == callback)
				{
					m_Listeners.erase(iterator);
					return;
				}
			}
		}

		void Invoke(Args... args)
		{
			for (auto& callback : m_Listeners)
			{
				callback(args...);
			}
		}

	private:
		std::vector<Callback> m_Listeners;
	};

}

