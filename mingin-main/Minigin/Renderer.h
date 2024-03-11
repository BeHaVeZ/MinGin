#pragma once
#include <SDL.h>
#include "Singleton.h"
#include "pch.h"

namespace dae
{
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_renderer{};
		SDL_Window* m_window{};
		SDL_Color m_clearColor{};	


		/// IMGUI EXERCISE
	private:
		void RenderImGuiEx1() const;
		void RenderImGuiEx2() const;
		mutable std::vector<float> X_DATA{};
		mutable std::vector<float> vec1;
		mutable std::vector<float> vec2;
		mutable std::vector<float> vec3;


	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

