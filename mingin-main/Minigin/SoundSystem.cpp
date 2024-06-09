#include "pch.h"

#include "SoundSystem.h"



using namespace dae;

#pragma region pImpl
class SDL_SoundSystem::SDL_SoundSystemImpl final
{
public:
	SDL_SoundSystemImpl()
	{
		StartUp();
	};
	~SDL_SoundSystemImpl()
	{
		Shutdown();
	};

	void PlaySound(const SoundId id, const float volume)
	{
		if (m_IsShutdown || m_IsMuted) return;

		m_Pending.push({ id, volume });
		m_Cv.notify_all();
	}

	void AddSound(const std::string& path, const SoundId id, bool doLoop = false)
	{
		if (m_IsShutdown) return;

		m_Sounds.emplace(id, Sound{ "../Data/Sounds/" + path, nullptr, false, doLoop });
	}

	void ToggleMute()
	{
		m_IsMuted = !m_IsMuted;
		for (auto& sound : m_Sounds)
		{
			if (sound.second.isLoaded)
			{
				Mix_VolumeChunk(sound.second.pChunk, m_IsMuted ? 0 : MIX_MAX_VOLUME);
			}
		}
	}

	void StartUp()
	{
		if (!m_IsShutdown) return;

		Mix_Init(MIX_INIT_OGG | MIX_INIT_MP3);
		Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
		m_UpdateThread = std::jthread(&SDL_SoundSystemImpl::Update, this);

		m_IsShutdown = false;
	}

	void Shutdown()
	{
		if (m_IsShutdown) return;

		m_IsShutdown = true;

		// Also stop all playing sounds
		for (auto& sound : m_Sounds)
		{
			if (sound.second.isLoaded)
			{
				Mix_FreeChunk(sound.second.pChunk);
				sound.second.isLoaded = false;
			}
		}

		Mix_CloseAudio();
		Mix_Quit();

		// Wake up thread to shut down
		m_Cv.notify_all();
	}

	bool IsShutdown()
	{
		return m_IsShutdown;
	}

private:
	struct PlayMessage
	{
		SoundId id;
		float volume;
	};

	struct Sound
	{
		std::string path;
		Mix_Chunk* pChunk;
		bool isLoaded;
		bool doLoop;
	};

	std::map<SoundId, Sound> m_Sounds;
	std::queue<PlayMessage> m_Pending;

	std::condition_variable m_Cv;
	std::mutex m_CvMutex;

	std::jthread m_UpdateThread;

	bool m_IsShutdown{ true };
	bool m_IsMuted{ false };

	void Update()
	{
		while (true)
		{
			std::unique_lock<std::mutex> lk(m_CvMutex);
			m_Cv.wait(lk, [&] {return !m_Pending.empty() || m_IsShutdown; });

			if (m_IsShutdown) return;

			auto& sound = m_Sounds[m_Pending.front().id];
			if (!sound.isLoaded)
			{
				sound.pChunk = Mix_LoadWAV(sound.path.c_str());
				sound.isLoaded = true;
			}

			sound.pChunk->volume = static_cast<uint8_t>(m_Pending.front().volume);
			Mix_PlayChannel((m_Pending.front().id + 1) % MIX_CHANNELS, sound.pChunk, -(int)sound.doLoop);

			m_Pending.pop();
		}
	};
};

SDL_SoundSystem::SDL_SoundSystem()
{
	m_pImpl = std::make_unique<SDL_SoundSystemImpl>();
}
void SDL_SoundSystem::PlaySound(const SoundId id, const float volume)
{
	m_pImpl->PlaySound(id, volume);
}

void SDL_SoundSystem::AddSound(const std::string& path, const SoundId id, bool doLoop)
{
	m_pImpl->AddSound(path, id, doLoop);
}

void SDL_SoundSystem::StartUp()
{
	m_pImpl->StartUp();
}

void SDL_SoundSystem::Shutdown()
{
	m_pImpl->Shutdown();
}

bool SDL_SoundSystem::IsShutdown()
{
	return m_pImpl->IsShutdown();
}

void SDL_SoundSystem::ToggleMute()
{
	m_pImpl->ToggleMute();
}
#pragma endregion

#pragma region Logging_SoundSystem
void Logging_SoundSystem::PlaySound(const SoundId id, const float volume)
{
	m_pSS->PlaySound(id, volume);
	LOG_TRACE("Sound with ID: " << id << " queued with " << volume << " set as volume");
}

void Logging_SoundSystem::AddSound(const std::string& path, const SoundId id, bool doLoop)
{
	m_pSS->AddSound(path, id, doLoop);
	LOG_TRACE("Sound with ID: " << id << " added with path ../Data/Sounds/" << path);
}

void Logging_SoundSystem::StartUp()
{
	m_pSS->StartUp();
	LOG_TRACE("Logging SoundSystem Started");
}

void Logging_SoundSystem::Shutdown()
{
	m_pSS->Shutdown();
	LOG_TRACE("Logging SoundSystem ended");
}

bool Logging_SoundSystem::IsShutdown()
{
	bool isShutdown{ m_pSS->IsShutdown() };
	LOG_INFO("SoundSystem is " << (isShutdown ? "down" : "alive"));
	return isShutdown;
}

void Logging_SoundSystem::ToggleMute()
{
	m_pSS->ToggleMute();
	LOG_TRACE("SoundSystem mute state toggled");
}
#pragma endregion