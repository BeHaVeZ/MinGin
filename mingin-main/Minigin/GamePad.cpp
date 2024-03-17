#include "pch.h"

#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")


namespace dae
{
    class GamePad::GamePadImpl
    {
        XINPUT_STATE previousState{};
        XINPUT_STATE currentState{};

        WORD buttonsPresssedThisFrame;
        WORD buttonsReleasedThisFrame;

        int _controllerIdx;
    public:
        GamePadImpl(int controllerIdx)
        {
            ZeroMemory(&previousState, sizeof(XINPUT_STATE));
            ZeroMemory(&currentState, sizeof(XINPUT_STATE));
            _controllerIdx = controllerIdx;
        }
        ~GamePadImpl() = default;


        void Update()
        {
            CopyMemory(&previousState, &currentState, sizeof(XINPUT_STATE));
            ZeroMemory(&currentState, sizeof(XINPUT_STATE));
            XInputGetState(_controllerIdx, &currentState);

            auto buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
            buttonsPresssedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
            buttonsPresssedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);
        }

        bool IsDownThisFrame(unsigned int button) const { return buttonsPresssedThisFrame & button; }
        bool IsUpThisFrame(unsigned int button) const { return buttonsReleasedThisFrame & button; }
        bool IsPressed(unsigned int button) const { return currentState.Gamepad.wButtons & button; }
    };
    GamePad::GamePad(int controllerIdx)
    {
        m_pImpl = new GamePadImpl(controllerIdx);
    }

    GamePad::~GamePad()
    {
        delete m_pImpl;
    }

    void GamePad::Update()
    {
        m_pImpl->Update();
    }

    bool GamePad::IsDown(ControllerButton button) const
    {
        return m_pImpl->IsDownThisFrame(static_cast<unsigned int>(button));
    }

    bool GamePad::IsUp(ControllerButton button) const
    {
        return m_pImpl->IsUpThisFrame(static_cast<unsigned int>(button));
    }

    bool GamePad::IsPressed(ControllerButton button) const
    {
        return m_pImpl->IsPressed(static_cast<unsigned int>(button));
    }
}