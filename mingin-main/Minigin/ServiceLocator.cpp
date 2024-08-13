#include "pch.h"
#include "ServiceLocator.h"

namespace dae
{
	std::unique_ptr<SoundSystem> ServiceLocator::m_pSSInstance{};
	NullSoundSystem ServiceLocator::m_DefaultSS;
}