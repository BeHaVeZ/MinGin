#pragma once
#include "Component.h"
#include "Texture2D.h"
#include <string>
#include <memory>

namespace dae
{
    class TextureComponent : public Component
    {
    public:
        TextureComponent();
        explicit TextureComponent(const std::string& filename);
        TextureComponent(const std::string& filename, float x, float y, float width = 0, float height = 0, float srcX = 0, float srcY = 0, float srcWidth = 0, float srcHeight = 0);
        virtual ~TextureComponent() = default;

        TextureComponent(const TextureComponent& other) = delete;
        TextureComponent(TextureComponent&& other) = delete;
        TextureComponent& operator=(const TextureComponent& other) = delete;
        TextureComponent& operator=(TextureComponent&& other) = delete;

        void Update() override {}
        void Render() const override;

        void SetPosition(float x, float y);
        void SetSize(float width, float height);
        void SetTexture(const std::string& filename);
        void SetSrcRectangle(float x, float y, float width, float height);

        float GetPosX() const { return m_PosX; }
        float GetPosY() const { return m_PosY; }
        float GetWidth() const { return m_Width; }
        float GetHeight() const { return m_Height; }

    private:
        std::shared_ptr<Texture2D> m_Texture;
        float m_PosX{}, m_PosY{}, m_Width{}, m_Height{};
        float m_SrcX{}, m_SrcY{}, m_SrcWidth{}, m_SrcHeight{};
    };
}
