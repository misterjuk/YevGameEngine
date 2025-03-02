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
#include "RotatorComponent.h"

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	auto background = std::make_unique<dae::GameObject>();

	background->AddComponent<dae::RenderComponent>(background.get());
	background->GetComponent<dae::RenderComponent>()->SetTexture("background.tga");
	scene.Add(std::move(background));

	auto logo = std::make_unique<dae::GameObject>();
	logo->AddComponent<dae::TransformComponent>(logo.get());
	logo->GetComponent<dae::TransformComponent>()->SetLocalPosition(216, 180,0);
	logo->AddComponent<dae::RotatorComponent>(logo.get());
	logo->GetComponent<dae::RotatorComponent>()->SetRadius(50.0f);
	logo->AddComponent<dae::RenderComponent>(logo.get());
	logo->GetComponent<dae::RenderComponent>()->SetTexture("logo.tga");
	

	auto logo2 = std::make_unique<dae::GameObject>();
	logo2->AddComponent<dae::TransformComponent>(logo2.get());
	logo2->GetComponent<dae::TransformComponent>()->SetLocalPosition(0, 0.0f, 0);
	logo2->SetParent(logo.get(), false);
	logo2->AddComponent<dae::RotatorComponent>(logo2.get());
	logo2->GetComponent<dae::RotatorComponent>()->SetRadius(50.0f);
	logo2->GetComponent<dae::RotatorComponent>()->SetSpeed(4.0f);
	logo2->AddComponent<dae::RenderComponent>(logo2.get());
	logo2->GetComponent<dae::RenderComponent>()->SetTexture("logo.tga");

	scene.Add(std::move(logo));
	scene.Add(std::move(logo2));






	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto text = std::make_unique<dae::GameObject>();
	text->AddComponent<dae::TransformComponent>(text.get());
	text->GetComponent<dae::TransformComponent>()->SetLocalPosition(80, 20, 0 );
	text->AddComponent<dae::TextComponent>(text.get(), "Programming 4 Assignment", font);
	scene.Add(std::move(text));

	

	auto fps = std::make_unique<dae::GameObject>();
	fps->AddComponent<dae::TransformComponent>(fps.get());
	fps->GetComponent<dae::TransformComponent>()->SetLocalPosition(0, 100, 0);
	fps->AddComponent<dae::TextComponent>(fps.get(), font);
	fps->AddComponent<dae::FPSComponent>(fps.get());
	scene.Add(std::move(fps));
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}