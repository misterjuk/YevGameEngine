#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"

#include "GameObject.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto background = std::make_unique<dae::GameObject>();

	background->AddComponent<dae::RenderComponent>(background.get());
	background->GetComponent<dae::RenderComponent>()->SetTexture("background.tga");
	scene.Add(std::move(background));

	auto logo = std::make_unique<dae::GameObject>();
	logo->AddComponent<dae::TransformComponent>(logo.get());
	logo->GetComponent<dae::TransformComponent>()->SetPosition(216, 180,0);
	logo->AddComponent<dae::RenderComponent>(logo.get());
	logo->GetComponent<dae::RenderComponent>()->SetTexture("logo.tga");
	scene.Add(std::move(logo));

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto text = std::make_unique<dae::GameObject>();
	text->AddComponent<dae::TransformComponent>(text.get());
	text->GetComponent<dae::TransformComponent>()->SetPosition(80, 20, 0 );
	text->AddComponent<dae::TextComponent>(text.get(), "Programming 4 Assignment", font);
	scene.Add(std::move(text));

	

	auto fps = std::make_unique<dae::GameObject>();
	fps->AddComponent<dae::TransformComponent>(fps.get());
	fps->GetComponent<dae::TransformComponent>()->SetPosition(0, 100, 0);
	fps->AddComponent<dae::TextComponent>(fps.get(), font);
	fps->AddComponent<dae::FPSComponent>(fps.get());
	scene.Add(std::move(fps));
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}