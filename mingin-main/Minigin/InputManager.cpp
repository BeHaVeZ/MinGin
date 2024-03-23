#include "pch.h"

bool dae::InputManager::ProcessInput()
{
	auto controllers = Input::GetInstance().GetControllers();
	auto keyboard = Input::GetInstance().GetKeyboard();
	auto controllerCommands = Input::GetInstance().GetControllerCommands();
	auto keyBoardCommands = Input::GetInstance().GetKeyboardCommands();


	for (auto& controller : controllers)
	{
		controller->Update();
	}

	for (const auto& controllerCommandPair : controllerCommands)
	{
		const auto& controller = controllers[std::get<0>(controllerCommandPair.first)];
		auto button = std::get<1>(controllerCommandPair.first);
		switch (std::get<2>(controllerCommandPair.first))
		{
		case KeyState::Down:
			if (controller->IsDown(button))
			{
				controllerCommandPair.second->Execute();
			}
			break;
		case KeyState::Pressed:
			if (controller->IsPressed(button))
			{
				controllerCommandPair.second->Execute();
			}
			break;
		case KeyState::Up:
			if (controller->IsUp(button))
			{
				controllerCommandPair.second->Execute();
			}
			break;
		default:
			break;
		}
	}
	keyboard->Update();

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}

		//Imguiprocessevent if imgui is needed/wanted (no)
	}

	for (const auto& keyValue : keyBoardCommands)
	{
		auto button = keyValue.first.first;
		switch (keyValue.first.second)
		{
		case SDL_KEYDOWN:
			if (keyboard->IsDown(button))
			{
				keyValue.second->Execute();
			}
			break;
		case SDL_KEYUP:
			if (keyboard->IsUp(button))
			{
				keyValue.second->Execute();
			}
			break;
		case SDL_KEYMAPCHANGED:
			if (keyboard->IsPressed(button))
			{
				keyValue.second->Execute();
			}
			break;
		}
	}

	return true;
}
