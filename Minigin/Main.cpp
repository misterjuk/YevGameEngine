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

#include "Map.h"
#include "GridMoveCommands.h"
#include "ServiceLocator.h"
#include "GameCommands.h"

void load()
{

	yev::ServiceLocator::GetInstance().GetSoundSystem()->LoadSoundEffect(0, "Sound/MainMusic.wav", 8, 5);
	
	
	auto& sceneManager = yev::SceneManager::GetInstance();

	auto& mainMenu = yev::SceneManager::GetInstance().CreateScene("MainMenu");

	auto hugefont = yev::ResourceManager::GetInstance().LoadFont("Lingua.otf", 120);

	auto font = yev::ResourceManager::GetInstance().LoadFont("Lingua.otf", 50);

	auto digdug = std::make_unique<yev::GameObject>();
	digdug->AddComponent<yev::TransformComponent>(digdug.get());
	digdug->GetComponent<yev::TransformComponent>()->SetLocalPosition(450, 100, 0);
	digdug->AddComponent<yev::TextComponent>(digdug.get(), "DigDug", hugefont);

	auto controls1 = std::make_unique<yev::GameObject>();
	controls1->AddComponent<yev::TransformComponent>(controls1.get());
	controls1->GetComponent<yev::TransformComponent>()->SetLocalPosition(100, 300, 0);
	controls1->AddComponent<yev::TextComponent>(controls1.get(), "WASD/DPAD - movement", font);

	auto controls2 = std::make_unique<yev::GameObject>();
	controls2->AddComponent<yev::TransformComponent>(controls2.get());
	controls2->GetComponent<yev::TransformComponent>()->SetLocalPosition(100, 400, 0);
	controls2->AddComponent<yev::TextComponent>(controls2.get(), "F/X(GamePad) - shooting (no visual)", font);

	auto controls3 = std::make_unique<yev::GameObject>();
	controls3->AddComponent<yev::TransformComponent>(controls3.get());
	controls3->GetComponent<yev::TransformComponent>()->SetLocalPosition(100, 500, 0);
	controls3->AddComponent<yev::TextComponent>(controls3.get(), "F1/RB - skip levels", font);

	auto controls4 = std::make_unique<yev::GameObject>();
	controls4->AddComponent<yev::TransformComponent>(controls4.get());
	controls4->GetComponent<yev::TransformComponent>()->SetLocalPosition(100, 600, 0);
	controls4->AddComponent<yev::TextComponent>(controls4.get(), "F2/LB - mute sound", font);

	auto controls5 = std::make_unique<yev::GameObject>();
	controls5->AddComponent<yev::TransformComponent>(controls5.get());
	controls5->GetComponent<yev::TransformComponent>()->SetLocalPosition(100, 700, 0);
	controls5->AddComponent<yev::TextComponent>(controls5.get(), "Enter/A(GamePad) - start the game", font);

	mainMenu.Add(std::move(digdug));
	mainMenu.Add(std::move(controls1));
	mainMenu.Add(std::move(controls2));
	mainMenu.Add(std::move(controls3));
	mainMenu.Add(std::move(controls4));
	mainMenu.Add(std::move(controls5));
	


	auto& endScreen = yev::SceneManager::GetInstance().CreateScene("EndScreen");

	auto gameover = std::make_unique<yev::GameObject>();
	gameover->AddComponent<yev::TransformComponent>(gameover.get());
	gameover->GetComponent<yev::TransformComponent>()->SetLocalPosition(50, 100, 0);
	gameover->AddComponent<yev::TextComponent>(gameover.get(), "Thank you for playing", hugefont);

	auto digdugEndScreen = std::make_unique<yev::GameObject>();
	digdugEndScreen->AddComponent<yev::TransformComponent>(digdugEndScreen.get());
	digdugEndScreen->GetComponent<yev::TransformComponent>()->SetLocalPosition(450, 250, 0);
	digdugEndScreen->AddComponent<yev::TextComponent>(digdugEndScreen.get(), "DigDug", hugefont);

	auto controlsEndScreen = std::make_unique<yev::GameObject>();
	controlsEndScreen->AddComponent<yev::TransformComponent>(controlsEndScreen.get());
	controlsEndScreen->GetComponent<yev::TransformComponent>()->SetLocalPosition(100, 700, 0);
	controlsEndScreen->AddComponent<yev::TextComponent>(controlsEndScreen.get(), "Enter/A(GamePad) - start the game", font);

	endScreen.Add(std::move(gameover));
	endScreen.Add(std::move(digdugEndScreen));
	endScreen.Add(std::move(controlsEndScreen));

	sceneManager.SetActiveScene(&mainMenu);

	yev::InputManager& inputManager = yev::InputManager::GetInstance();

	inputManager.BindKeyboardCommand(SDLK_F2, yev::InputState::Released, std::make_unique<MuteSoundCommand>());
	inputManager.BindGamepadCommand(XINPUT_GAMEPAD_LEFT_SHOULDER, yev::InputState::Released, std::make_unique<MuteSoundCommand>());

	inputManager.BindKeyboardCommand(SDLK_F1, yev::InputState::Released, std::make_unique<SkipLevelCommand>());
	inputManager.BindGamepadCommand(XINPUT_GAMEPAD_RIGHT_SHOULDER, yev::InputState::Released, std::make_unique<SkipLevelCommand>());


	inputManager.BindKeyboardCommand(SDLK_RETURN, yev::InputState::Released, std::make_unique<StartGameCommand>());
	inputManager.BindGamepadCommand(XINPUT_GAMEPAD_A, yev::InputState::Released, std::make_unique<StartGameCommand>());
}

int main(int, char*[]) {
	yev::Minigin engine("Data/");
	engine.Run(load);
    return 0;
}

