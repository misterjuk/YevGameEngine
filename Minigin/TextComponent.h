#pragma once
#include <string>
#include <memory>
#include "RenderComponent.h"


namespace yev
{
	class Font;
	class Texture2D;

	class TextComponent final : public RenderComponent
	{
	public:


		void Update() override;

		void SetText(const std::string& text);

		TextComponent(GameObject* owner, const std::string& text, const std::shared_ptr<Font>& font);
		TextComponent(GameObject* owner, const std::shared_ptr<Font>& font);
		virtual ~TextComponent() = default;

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		bool m_needsUpdate;
		std::string m_text;
		std::shared_ptr<Font> m_font;
	};
};
