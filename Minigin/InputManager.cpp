#include "InputManager.h"
#include "Gamepad.h"
#include <SDL.h>
#include <algorithm>

namespace yev
{
    InputManager::InputManager()
        : m_Gamepad(std::make_unique<Gamepad>())
    {

        // Initialize previous key states with a default value (false)
        const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
        SDL_Keycode keys[] = {
            SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, // Add other keys as needed
            // List all the keys you are interested in
        };

        for (SDL_Keycode key : keys)
        {
            m_PreviousKeyStates[key] = keyboardState[SDL_GetScancodeFromKey(key)] != 0;
        }
    }

    InputManager::~InputManager() = default;

    bool InputManager::ProcessInput()
    {
        // Update and process controller input
        m_Gamepad->Update();



        SDL_Event e;
        std::unordered_map<SDL_Keycode, bool> currentKeyStates;
        std::unordered_map<SDL_Keycode, bool> previousKeyStates;

        // Initialize key states for the frame
        const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
        for (const auto& [key, commandPair] : m_KeyboardCommands)
        {
            currentKeyStates[key] = keyboardState[SDL_GetScancodeFromKey(key)] != 0;
            previousKeyStates[key] = m_PreviousKeyStates[key];
        }

        // Process SDL events
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                return false;

            if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
            {
                SDL_Keycode key = e.key.keysym.sym;
                bool keyIsPressed = (e.type == SDL_KEYDOWN);
                currentKeyStates[key] = keyIsPressed;
            }
        }

        // Update commands based on key state changes
        for (const auto& [key, commandPair] : m_KeyboardCommands)
        {
            const auto& [state, command] = commandPair;
            bool keyIsPressed = currentKeyStates[key];
            bool keyWasPressed = previousKeyStates[key];

            if (state == InputState::Pressed && keyIsPressed && !keyWasPressed)
            {
                command->Execute();
            }
            else if (state == InputState::Released && !keyIsPressed && keyWasPressed)
            {
                command->Execute();
            }
            else if (state == InputState::Held && keyIsPressed)
            {
                command->Execute();
            }
        }

        // Store the current key states for the next frame
        m_PreviousKeyStates = currentKeyStates;



        return true;
    }

    void InputManager::BindKeyboardCommand(SDL_Keycode key, InputState state, std::unique_ptr<Command> command)
    {
        m_KeyboardCommands[key] = std::make_pair(state, std::move(command));


    }

    void InputManager::BindGamepadCommand(unsigned int button, InputState state, std::unique_ptr<Command> command)
    {
        m_Gamepad->BindControllerCommand(button, state, std::move(command));
    }

    bool InputManager::IsPressed(SDL_Keycode key)
    {
        const Uint8* state = SDL_GetKeyboardState(nullptr);
        return state[SDL_GetScancodeFromKey(key)] != 0;
    }
}