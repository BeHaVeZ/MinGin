#include "pch.h"

using std::chrono::high_resolution_clock, std::chrono::duration;

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
		printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
			version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string& dataPath)
{
	PrintSDLVersion();

	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);

	SDL_GetCurrentDisplayMode(0, &current);
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();
	auto& input = InputManager::GetInstance();
	auto& timer = Timer::GetInstance();

	auto last_time = high_resolution_clock::now();
	float lag = 0.f;
	const unsigned int refreshRate = current.refresh_rate;
	const float ms_per_frame = 1000.0f / static_cast<float>(refreshRate);

	while (true)
	{
		const auto current_time = high_resolution_clock::now();
		const float delta_time = duration<float>(current_time - last_time).count();
		last_time = current_time;
		lag += delta_time;

		timer.Update();
		if (!input.ProcessInput())
			break;

		//while (lag >= fixed_time_step)
		//{
		//	// TODO: Fixed updates voor physics en networking
		//	// fixedUpdate(fixed_time_step);
		//	lag -= fixed_time_step;
		//}
		//feedback time berekenen eerst en dan processinput
		// componenten ook removen, het moet worden toegevoegd!!!
		sceneManager.Update();
		renderer.Render();

		const auto sleep_time = current_time + std::chrono::milliseconds(static_cast<int>(ms_per_frame)) - high_resolution_clock::now();

		if (sleep_time > std::chrono::milliseconds(0))
		{
			std::this_thread::sleep_for(sleep_time);
		}
	}
}
