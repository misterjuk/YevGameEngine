#include <stdexcept>
#include <SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

namespace yev 
{
	TextComponent::TextComponent(GameObject* owner, const std::shared_ptr<Font>& font)
		: TextComponent(owner, "Text", font) 
	{
	}

	TextComponent::TextComponent(GameObject* owner, const std::string& text, const std::shared_ptr<Font>& font)
		: RenderComponent(owner), m_needsUpdate(true), m_text(text), m_font(font)
	{
	}

	void TextComponent::Update()
	{
		if (m_needsUpdate)
		{
			const SDL_Color color = { 255,255,255,255 }; // only white text is supported now
			const auto surf = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), color);
			if (surf == nullptr)
			{
				throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
			}
			auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
			if (texture == nullptr)
			{
				throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
			}
			SDL_FreeSurface(surf);
			m_texture = std::make_shared<Texture2D>(texture);
			m_needsUpdate = false;
		}
	}

	void TextComponent::SetText(const std::string& text)
	{
		m_text = text;
		m_needsUpdate = true;
	}

}



