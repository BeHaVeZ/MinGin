#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "Component.h"

namespace dae
{
	class Font;
	class Texture2D;

	class TextObject : public Component
	{
	public:
		void Update() override;
		void Render() const override;

		void SetText(const std::string& text);
		void SetPosition(float x, float y);

		TextObject(const std::string& text, std::shared_ptr<Font> font);
		virtual ~TextObject();
		TextObject(const TextObject& other) = delete;
		TextObject(TextObject&& other) = delete;
		TextObject& operator=(const TextObject& other) = delete;
		TextObject& operator=(TextObject&& other) = delete;

	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		Transform m_Transform{};
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_TextTexture;

		//glm::vec3 m_ParentPosition;
	};
}
