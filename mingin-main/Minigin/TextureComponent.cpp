#include "pch.h"
#include "TextureComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"

namespace dae
{
    TextureComponent::TextureComponent()
        : m_Texture(nullptr)
    {}

    TextureComponent::TextureComponent(const std::string& filename)
    {
        SetTexture(filename);
    }

    TextureComponent::TextureComponent(const std::string& filename, float x, float y, float width, float height, float srcX, float srcY, float srcWidth, float srcHeight)
        : m_PosX{ x }
        , m_PosY{ y }
        , m_Width(width)
        , m_Height(height)
        , m_SrcX(srcX)
        , m_SrcY(srcY)
        , m_SrcWidth(srcWidth)
        , m_SrcHeight(srcHeight)
    {
        SetTexture(filename);
    }

    void TextureComponent::Render() const
    {
        if (m_SrcWidth > 0 && m_SrcHeight > 0)
        {
            Renderer::GetInstance().RenderTexture(*m_Texture, m_PosX, m_PosY, m_Width, m_Height, m_SrcX, m_SrcY, m_SrcWidth, m_SrcHeight);
        }
        else
        {
            if (m_Width > 0 && m_Height > 0)
                Renderer::GetInstance().RenderTexture(*m_Texture, m_PosX, m_PosY, m_Width, m_Height);
            else
                Renderer::GetInstance().RenderTexture(*m_Texture, m_PosX, m_PosY);
        }
    }

    void TextureComponent::SetPosition(float x, float y)
    {
        m_PosX = x;
        m_PosY = y;
    }

    void TextureComponent::SetSize(float width, float height)
    {
        m_Width = width;
        m_Height = height;
    }

    void TextureComponent::SetTexture(const std::string& filename)
    {
        m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
    }

    void TextureComponent::SetSrcRectangle(float x, float y, float width, float height)
    {
        m_SrcX = x;
        m_SrcY = y;
        m_SrcWidth = width;
        m_SrcHeight = height;
    }
}
