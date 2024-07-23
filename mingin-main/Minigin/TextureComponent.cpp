#include "pch.h"

namespace dae
{
	TextureComponent::TextureComponent(const std::string& filename)
		: m_Texture(ResourceManager::GetInstance().LoadTexture(filename))
	{}

	void TextureComponent::Render() const
	{
		if (GetGameObject())
		{
			const auto& pos = GetGameObject()->GetTransform().GetPosition();
			Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
		}
	}
}