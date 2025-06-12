#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace yev
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void SetActiveScene(Scene* scene);
		Scene* GetActiveScene() const { return m_activeScene; }

		void Update();
		void FixedUpdate();
		void Render();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;

		Scene* m_activeScene{ nullptr };
	};
}
