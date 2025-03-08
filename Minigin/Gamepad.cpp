#include "Gamepad.h"
#include <Xinput.h>
#include <unordered_map>
#include <memory>

namespace dae
{
    // Constructor initializes the controller state
    Gamepad::Gamepad()
    {
        ZeroMemory(&m_ControllerState, sizeof(XINPUT_STATE));
    }

    // Destructor
    Gamepad::~Gamepad() = default;

    // Update method to handle controller input
    void Gamepad::Update()
    {
        ZeroMemory(&m_ControllerState, sizeof(XINPUT_STATE));
        XInputGetState(0, &m_ControllerState); // Retrieve controller state

        for (const auto& [button, commandPair] : m_ControllerCommands)
        {
            const auto& [state, command] = commandPair;
            bool isPressed = IsPressed(button);

            // Determine if the button's state should trigger the command
            switch (state)
            {
            case InputState::Pressed:
                if (isPressed && !m_PreviousButtonStates[button]) // Button just pressed
                {
                    command->Execute();
                }
                break;

            case InputState::Released:
                if (!isPressed && m_PreviousButtonStates[button]) // Button just released
                {
                    command->Execute();
                }
                break;

            case InputState::Held:
                if (isPressed) // Button held down
                {
                    command->Execute();
                }
                break;
            }
        }

        // Update previous button states
        for (auto& [button, commandPair] : m_ControllerCommands)
        {
            const auto& [state, command] = commandPair;
            m_PreviousButtonStates[button] = IsPressed(button);
        }
    }

    // Bind a command to a button with a specific input state
    void Gamepad::BindControllerCommand(unsigned int button, InputState state, std::unique_ptr<Command> command)
    {
        m_ControllerCommands[button] = std::make_pair(state, std::move(command));
        // Ensure the previous button state is initialized
        if (m_PreviousButtonStates.find(button) == m_PreviousButtonStates.end())
        {
            m_PreviousButtonStates[button] = IsPressed(button);
        }
    }

    // Check if a button is pressed
    bool Gamepad::IsPressed(unsigned int button)
    {
        return (m_ControllerState.Gamepad.wButtons & button) != 0;
    }
}
