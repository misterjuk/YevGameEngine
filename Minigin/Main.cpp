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
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "HealthDisplayComponent.h"
#include "ScoreDisplayComponent.h"

void bindPlayersInput(yev::GameObject* character,yev::GameObject* character2);

void load()
{
	auto& scene = yev::SceneManager::GetInstance().CreateScene("Demo");

	auto background = std::make_unique<yev::GameObject>();

	background->AddComponent<yev::RenderComponent>(background.get());
	background->GetComponent<yev::RenderComponent>()->SetTexture("background.tga");
	scene.Add(std::move(background));

	
	auto font = yev::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

	auto text = std::make_unique<yev::GameObject>();
	text->AddComponent<yev::TransformComponent>(text.get());
	text->GetComponent<yev::TransformComponent>()->SetLocalPosition(80, 20, 0 );
	text->AddComponent<yev::TextComponent>(text.get(), "Programming 4 Assignment", font);
	scene.Add(std::move(text));

	



	auto smallfont = yev::ResourceManager::GetInstance().LoadFont("Lingua.otf", 20);

	auto scoreDisplay1 = std::make_unique<yev::GameObject>();
	scoreDisplay1->AddComponent<yev::TransformComponent>(scoreDisplay1.get());
	scoreDisplay1->GetComponent<yev::TransformComponent>()->SetLocalPosition(400, 300, 0);
	scoreDisplay1->AddComponent<yev::TextComponent>(scoreDisplay1.get(), "Score", smallfont);
	scoreDisplay1->AddComponent<ScoreDisplayComponent>(scoreDisplay1.get(), scoreDisplay1->GetComponent<yev::TextComponent>());
	

	auto scoreDisplay2 = std::make_unique<yev::GameObject>();
	scoreDisplay2->AddComponent<yev::TransformComponent>(scoreDisplay2.get());
	scoreDisplay2->GetComponent<yev::TransformComponent>()->SetLocalPosition(400, 350, 0);
	scoreDisplay2->AddComponent<yev::TextComponent>(scoreDisplay2.get(), "Score", smallfont);
	scoreDisplay2->AddComponent<ScoreDisplayComponent>(scoreDisplay2.get(), scoreDisplay2->GetComponent<yev::TextComponent>());
	


	auto healthDisplay = std::make_unique<yev::GameObject>();
	healthDisplay->AddComponent<yev::TransformComponent>(healthDisplay.get());
	healthDisplay->GetComponent<yev::TransformComponent>()->SetLocalPosition(400, 400, 0);
	healthDisplay->AddComponent<yev::TextComponent>(healthDisplay.get(), "Health", smallfont);
	healthDisplay->AddComponent<HealthDisplayComponent>(healthDisplay.get(), healthDisplay->GetComponent<yev::TextComponent>());
	

	auto healthDisplay2 = std::make_unique<yev::GameObject>();
	healthDisplay2->AddComponent<yev::TransformComponent>(healthDisplay2.get());
	healthDisplay2->GetComponent<yev::TransformComponent>()->SetLocalPosition(400, 450, 0);
	healthDisplay2->AddComponent<yev::TextComponent>(healthDisplay2.get(), "Health", smallfont);
	healthDisplay2->AddComponent<HealthDisplayComponent>(healthDisplay2.get(), healthDisplay2->GetComponent<yev::TextComponent>());
	

	auto tutorial = std::make_unique<yev::GameObject>();
	tutorial->AddComponent<yev::TransformComponent>(tutorial.get());
	tutorial->GetComponent<yev::TransformComponent>()->SetLocalPosition(0, 60, 0);
	tutorial->AddComponent<yev::TextComponent>(tutorial.get(), "WASD to move DigDug, C to inflict damage, Z to increase score", smallfont);
	scene.Add(std::move(tutorial));

	auto tutorial2 = std::make_unique<yev::GameObject>();
	tutorial2->AddComponent<yev::TransformComponent>(tutorial2.get());
	tutorial2->GetComponent<yev::TransformComponent>()->SetLocalPosition(0, 80, 0);
	tutorial2->AddComponent<yev::TextComponent>(tutorial2.get(), "D-pad to move Enemy, X to inflict damage, A to increase score", smallfont);
	scene.Add(std::move(tutorial2));
	

	auto fps = std::make_unique<yev::GameObject>();
	fps->AddComponent<yev::TransformComponent>(fps.get());
	fps->GetComponent<yev::TransformComponent>()->SetLocalPosition(0, 100, 0);
	fps->AddComponent<yev::TextComponent>(fps.get(), font);
	fps->AddComponent<yev::FPSComponent>(fps.get());
	scene.Add(std::move(fps));



	auto character = std::make_unique<yev::GameObject>();
	character->AddComponent<yev::TransformComponent>(character.get());
	character->GetComponent<yev::TransformComponent>()->SetLocalPosition(150, 180, 0);
	character->GetComponent<yev::TransformComponent>()->SetScale(glm::vec3(3.0f, 3.0f, 1.0f));

	character->AddComponent<ScoreComponent>(character.get());
	character->GetComponent<ScoreComponent>()->AddObserver(scoreDisplay1->GetComponent<ScoreDisplayComponent>());
	character->AddComponent<HealthComponent>(character.get());
	character->GetComponent<HealthComponent>()->AddObserver(healthDisplay->GetComponent<HealthDisplayComponent>());
	character->AddComponent<yev::RenderComponent>(character.get());
	character->GetComponent<yev::RenderComponent>()->SetTexture("Player.png");


	auto character2 = std::make_unique<yev::GameObject>();
	character2->AddComponent<yev::TransformComponent>(character2.get());
	character2->GetComponent<yev::TransformComponent>()->SetLocalPosition(300, 180.0f, 0);
	character2->GetComponent<yev::TransformComponent>()->SetScale(glm::vec3(3.0f, 3.0f, 1.0f));

	character2->AddComponent<ScoreComponent>(character2.get());
	character2->GetComponent<ScoreComponent>()->AddObserver(scoreDisplay2->GetComponent<ScoreDisplayComponent>());
	character2->AddComponent<HealthComponent>(character2.get());
	character2->GetComponent<HealthComponent>()->AddObserver(healthDisplay2->GetComponent<HealthDisplayComponent>());
	character2->AddComponent<yev::RenderComponent>(character2.get());
	character2->GetComponent<yev::RenderComponent>()->SetTexture("Enemy.png");

	bindPlayersInput(character.get(), character2.get());

	scene.Add(std::move(character));
	scene.Add(std::move(character2));
	


	scene.Add(std::move(scoreDisplay1));
	scene.Add(std::move(scoreDisplay2));
	scene.Add(std::move(healthDisplay));
	scene.Add(std::move(healthDisplay2));
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


	std::unique_ptr<yev::ApplyScoreCommand> applyScoreChar1 = std::make_unique<yev::ApplyScoreCommand>(character);
	applyScoreChar1->AddObserver(character->GetComponent<ScoreComponent>());
	std::unique_ptr<yev::ApplyDamageCommand> applyDamageChar1 = std::make_unique<yev::ApplyDamageCommand>(character);
	applyDamageChar1->AddObserver(character->GetComponent<HealthComponent>());

	inputManager.BindKeyboardCommand(SDLK_z, yev::InputState::Pressed, std::move(applyScoreChar1));
	inputManager.BindKeyboardCommand(SDLK_c, yev::InputState::Pressed, std::move(applyDamageChar1));



   
	std::unique_ptr<yev::ApplyScoreCommand> applyScoreChar2 = std::make_unique<yev::ApplyScoreCommand>(character2);
	applyScoreChar2->AddObserver(character2->GetComponent<ScoreComponent>());
	std::unique_ptr<yev::ApplyDamageCommand> applyDamageChar2 = std::make_unique<yev::ApplyDamageCommand>(character2);
	applyDamageChar2->AddObserver(character2->GetComponent<HealthComponent>());

	inputManager.BindGamepadCommand(XINPUT_GAMEPAD_A, yev::InputState::Pressed, std::move(applyScoreChar2));
	inputManager.BindGamepadCommand(XINPUT_GAMEPAD_X, yev::InputState::Pressed, std::move(applyDamageChar2));

	inputManager.BindGamepadCommand(XINPUT_GAMEPAD_DPAD_LEFT, yev::InputState::Held, std::make_unique<yev::MoveLeftCommand>(character2, char2Speed));
	inputManager.BindGamepadCommand(XINPUT_GAMEPAD_DPAD_RIGHT, yev::InputState::Held, std::make_unique<yev::MoveRightCommand>(character2, char2Speed));
	inputManager.BindGamepadCommand(XINPUT_GAMEPAD_DPAD_UP, yev::InputState::Held, std::make_unique<yev::MoveUpCommand>(character2, char2Speed));
	inputManager.BindGamepadCommand(XINPUT_GAMEPAD_DPAD_DOWN, yev::InputState::Held, std::make_unique<yev::MoveDownCommand>(character2, char2Speed));
}