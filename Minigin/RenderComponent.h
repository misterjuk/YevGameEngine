#pragma once

#include "Component.h"
#include <string>
#include <memory>


namespace dae 
{

	class Texture2D;
	class TransformComponent;


	class RenderComponent : public Component
	{

	public:

		RenderComponent(GameObject* owner);
		RenderComponent(GameObject* owner, const std::shared_ptr<Texture2D>& texture);
		virtual ~RenderComponent() = default;

		void Render() const override;
		void SetTexture(const std::string& filename);

	protected:
		//load texture returns the shared ptr, so probably no need for uniqueptr
		std::shared_ptr<Texture2D> m_texture;

		TransformComponent* m_transform;

	};


}
