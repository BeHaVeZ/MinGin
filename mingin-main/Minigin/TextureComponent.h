#pragma once
#include "Component.h"
#include "Texture2D.h"
#include <string>
#include <memory>
#include "GameObject.h"

namespace dae
{
	class TextureComponent : public Component
	{
	public:
		explicit TextureComponent(const std::string& filename);
		virtual ~TextureComponent() = default;

		void Update() override {};
		void Render() const override;

	private:
		std::shared_ptr<Texture2D> m_Texture;
	};
}