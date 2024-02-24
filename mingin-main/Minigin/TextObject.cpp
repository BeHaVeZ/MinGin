#include <stdexcept>
#include <SDL_ttf.h>
#include "TextObject.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GameObject.h"
#include <iostream>

dae::TextObject::TextObject(const std::string& text, std::shared_ptr<Font> font)
    : m_NeedsUpdate(true), m_Text(text), m_Font(std::move(font)), m_TextTexture(nullptr)
{
}

void dae::TextObject::Update()
{
    if (m_NeedsUpdate)
    {
        const SDL_Color color = { 255, 255, 255, 255 }; // white text
        const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
        if (surf == nullptr)
        {
            throw std::runtime_error("Failed to render text: " + std::string(SDL_GetError()));
        }

        auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
        SDL_FreeSurface(surf);

        if (texture == nullptr)
        {
            throw std::runtime_error("Failed to create text texture from surface: " + std::string(SDL_GetError()));
        }

        m_TextTexture = std::make_shared<Texture2D>(texture);
        m_NeedsUpdate = false;
    }
}

void dae::TextObject::Render() const
{
    if (m_TextTexture != nullptr)
    {
        auto parent = GetGameObject();
        if (parent)
        {
            const auto& parentTransform = parent->GetTransform();
            const auto& pos = parentTransform.GetPosition();
            Renderer::GetInstance().RenderTexture(*m_TextTexture, pos.x + m_Transform.GetPosition().x, pos.y + m_Transform.GetPosition().y);
        }
    }
}

// This implementation uses the "dirty flag" pattern
void dae::TextObject::SetText(const std::string& text)
{
    if (m_Text != text)
    {
        m_Text = text;
        m_NeedsUpdate = true;
    }
}

void dae::TextObject::SetPosition(const float x, const float y)
{
    auto parent = GetGameObject();
    if (parent)
    {
        const auto& parentPosition = parent->GetTransform().GetPosition();
        m_Transform.SetPosition(parentPosition.x + x, parentPosition.y + y, 0.f);
    }
    else
    {
        m_Transform.SetPosition(x, y, 0.f);
    }
}

dae::TextObject::~TextObject()
{
    if (m_TextTexture != nullptr)
    {
        SDL_DestroyTexture(m_TextTexture->GetSDLTexture());
    }
}
