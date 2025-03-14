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

#include "InputManager.h"
#include "PlayerCommands.h"
#include "Gamepad.h"

#include "GameObject.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RotatorComponent.h"
#include "ScoreComponent.h"
#include "HealthDisplayComponent.h"

void bindPlayersInput(yev::GameObject* character,yev::GameObject* character2);

void load()
{
	auto& scene = yev::SceneManager::GetInstance().CreateScene("Demo");

	auto background = std::make_unique<yev::GameObject>();

	background->AddComponent<yev::RenderComponent>(background.get());
	background->GetComponent<yev::RenderComponent>()->SetTexture("background.tga");
	scene.Add(std::move(background));

	auto character = std::make_unique<yev::GameObject>();
	character->AddComponent<yev::TransformComponent>(character.get());
	character->GetComponent<yev::TransformComponent>()->SetLocalPosition(150, 180,0);
	character->GetComponent<yev::TransformComponent>()->SetScale(glm::vec3(3.0f,3.0f,1.0f));
	character->AddComponent<yev::RenderComponent>(character.get());
	character->GetComponent<yev::RenderComponent>()->SetTexture("Player.png");
	

	auto character2 = std::make_unique<yev::GameObject>();
	character2->AddComponent<yev::TransformComponent>(character2.get());
	character2->GetComponent<yev::TransformComponent>()->SetLocalPosition(300, 180.0f, 0);
	character2->GetComponent<yev::TransformComponent>()->SetScale(glm::vec3(3.0f, 3.0f, 1.0f));
	character2->AddComponent<yev::RenderComponent>(character2.get());
	character2->GetComponent<yev::RenderComponent>()->SetTexture("Enemy.png");

	bindPlayersInput(character.get(), character2.get());

	scene.Add(std::move(character));
	scene.Add(std::move(character2));


	auto font = yev::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto text = std::make_unique<yev::GameObject>();
	text->AddComponent<yev::TransformComponent>(text.get());
	text->GetComponent<yev::TransformComponent>()->SetLocalPosition(80, 20, 0 );
	text->AddComponent<yev::TextComponent>(text.get(), "Programming 4 Assignment", font);
	scene.Add(std::move(text));



	auto smallfont = yev::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	auto score = std::make_unique<yev::GameObject>();
	score->AddComponent<yev::TransformComponent>(score.get());
	score->GetComponent<yev::TransformComponent>()->SetLocalPosition(400, 300, 0);
	score->AddComponent<yev::TextComponent>(score.get(), "Score", smallfont);
	score->AddComponent<ScoreComponent>(score.get(), score->GetComponent<yev::TextComponent>());
	scene.Add(std::move(score));

	auto score2 = std::make_unique<yev::GameObject>();
	score2->AddComponent<yev::TransformComponent>(score2.get());
	score2->GetComponent<yev::TransformComponent>()->SetLocalPosition(400, 350, 0);
	score2->AddComponent<yev::TextComponent>(score2.get(), "Score", smallfont);
	score2->AddComponent<ScoreComponent>(score2.get(), score2->GetComponent<yev::TextComponent>());
	scene.Add(std::move(score2));


	auto healthDisplay = std::make_unique<yev::GameObject>();
	healthDisplay->AddComponent<yev::TransformComponent>(healthDisplay.get());
	healthDisplay->GetComponent<yev::TransformComponent>()->SetLocalPosition(400, 400, 0);
	healthDisplay->AddComponent<yev::TextComponent>(healthDisplay.get(), "Health", smallfont);
	healthDisplay->AddComponent<HealthDisplayComponent>(healthDisplay.get(), healthDisplay->GetComponent<yev::TextComponent>());
	scene.Add(std::move(healthDisplay));

	auto healthDisplay2 = std::make_unique<yev::GameObject>();
	healthDisplay2->AddComponent<yev::TransformComponent>(healthDisplay2.get());
	healthDisplay2->GetComponent<yev::TransformComponent>()->SetLocalPosition(400, 450, 0);
	healthDisplay2->AddComponent<yev::TextComponent>(healthDisplay2.get(), "Health", smallfont);
	healthDisplay2->AddComponent<HealthDisplayComponent>(healthDisplay2.get(), healthDisplay2->GetComponent<yev::TextComponent>());
	scene.Add(std::move(healthDisplay2));


	

	auto fps = std::make_unique<yev::GameObject>();
	fps->AddComponent<yev::TransformComponent>(fps.get());
	fps->GetComponent<yev::TransformComponent>()->SetLocalPosition(0, 100, 0);
	fps->AddComponent<yev::TextComponent>(fps.get(), font);
	fps->AddComponent<yev::FPSComponent>(fps.get());
	scene.Add(std::move(fps));



	
}

int main(int, char*[]) {
	yev::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}

void bindPlayersInput(yev::GameObject* character, yev::GameObject* character2)
{
	yev::InputManager& inputManager = yev::InputManager::GetInstance();


	//TODO make a component with basic variables for the players
	float char1Speed = 100.0f;
	float char2Speed = 200.0f;

	inputManager.BindKeyboardCommand(SDLK_a, yev::InputState::Held, std::make_unique<yev::MoveLeftCommand>(character, char1Speed));
	inputManager.BindKeyboardCommand(SDLK_d, yev::InputState::Held, std::make_unique<yev::MoveRightCommand>(character, char1Speed));
	inputManager.BindKeyboardCommand(SDLK_w, yev::InputState::Held, std::make_unique<yev::MoveUpCommand>(character, char1Speed));
	inputManager.BindKeyboardCommand(SDLK_s, yev::InputState::Held, std::make_unique<yev::MoveDownCommand>(character, char1Speed));

	inputManager.BindGamepadCommand(XINPUT_GAMEPAD_DPAD_LEFT, yev::InputState::Held, std::make_unique<yev::MoveLeftCommand>(character2, char2Speed));
	inputManager.BindGamepadCommand(XINPUT_GAMEPAD_DPAD_RIGHT, yev::InputState::Held, std::make_unique<yev::MoveRightCommand>(character2, char2Speed));
	inputManager.BindGamepadCommand(XINPUT_GAMEPAD_DPAD_UP, yev::InputState::Held, std::make_unique<yev::MoveUpCommand>(character2, char2Speed));
	inputManager.BindGamepadCommand(XINPUT_GAMEPAD_DPAD_DOWN, yev::InputState::Held, std::make_unique<yev::MoveDownCommand>(character2, char2Speed));
}