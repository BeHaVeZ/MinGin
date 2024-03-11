#include "pch.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void dae::Renderer::RenderImGuiEx1() const
{
	ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
	ImGui::SetWindowSize(ImVec2(500, 300));
	ImGui::Begin("Exercise 1");

	static int sampleCount = 10;
	ImGui::InputInt("##Samples", &sampleCount);

	ImGui::SameLine();
	ImGui::Text("#Samples");

	if (ImGui::Button("Trash the cache"))
	{
		X_DATA.clear();
		vec1.clear();
		int stepsize{ sampleCount };
		const int arrSize{ 10000000 };
		int* myArray = new int[arrSize]();

		for (stepsize = 1; stepsize <= 1024; stepsize *= 2) {
			auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < arrSize; i += stepsize) {
				myArray[i] *= 2;
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			vec1.push_back((float)elapsedTime);
			X_DATA.push_back((float)stepsize);
		}

		delete[] myArray;
	}
	ImPlot::SetNextAxesLimits(0, 512, 0, 2000);
	if (ImPlot::BeginPlot("Trash or Cache?"))
	{
		ImPlot::PlotLine("Performance in ints", X_DATA.data(), vec1.data(), static_cast<int>(vec1.size()));

		ImPlot::EndPlot();
	}
	ImGui::End();
}

void dae::Renderer::RenderImGuiEx2() const
{
	ImGui::SetNextWindowPos(ImVec2(100, 450), ImGuiCond_FirstUseEver);
	ImGui::SetWindowSize(ImVec2(300, 150));
	ImGui::Begin("Exercise 2");

	static int sampleCount = 100;
	ImGui::InputInt("##Samples", &sampleCount);

	ImGui::SameLine();
	ImGui::Text("#Samples");
	if (ImGui::Button("GameObject"))
	{
		X_DATA.clear();
		vec2.clear();
		int stepsize{ sampleCount };
		const int arrSize{ 10000000 };
		std::vector<gameobject> myContainer(arrSize);

		for (stepsize = 1; stepsize <= 1024; stepsize *= 2) {
			auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < arrSize; i += stepsize) {
				myContainer[i].id *= 2;
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			vec2.push_back((float)elapsedTime);
			X_DATA.push_back((float)stepsize);
		}
	}

	if (ImGui::Button("GameObjectAlt"))
	{
		X_DATA.clear();
		vec3.clear();
		int stepsize{ sampleCount };
		const int arrSize{ 10000000 };
		std::vector<gameobjectalt> myContainer(arrSize);

		for (stepsize = 1; stepsize <= 1024; stepsize *= 2) {
			auto start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < arrSize; i += stepsize) {
				myContainer[i].id *= 2;
			}
			auto end = std::chrono::high_resolution_clock::now();
			auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
			vec3.push_back((float)elapsedTime);
			X_DATA.push_back((float)stepsize);
		}
	}
	ImPlot::SetNextAxesLimits(0, 512, 0, 2000);
	if (ImPlot::BeginPlot("Trash or Cache?"))
	{
		ImPlot::PlotLine("gameobjects", X_DATA.data(), vec2.data(), static_cast<int>(vec2.size()));
		ImPlot::PlotLine("gameobjectsAlt", X_DATA.data(), vec3.data(), static_cast<int>(vec3.size()));

		ImPlot::EndPlot();
	}
	ImGui::End();



}

void dae::Renderer::Init(SDL_Window* window)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();


	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void dae::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	SceneManager::GetInstance().Render();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	RenderImGuiEx1();
	RenderImGuiEx2();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_renderer);
}

void dae::Renderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();


	ImPlot::DestroyContext();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

SDL_Renderer* dae::Renderer::GetSDLRenderer() const { return m_renderer; }
