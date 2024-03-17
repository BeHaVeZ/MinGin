#include "pch.h"

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	auto controllers = Input::GetInstance().GetControllers();
	auto keyBoardCommands = Input::GetInstance().GetKeyboardCommands();
	auto controllerCommands = Input::GetInstance().GetControllerCommands();


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

	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
		// etc...

		//Imguiprocessevent if imgui is needed/wanted (no)
	}

	const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
	for (const auto& keyValue : keyBoardCommands)
	{
		if (currentKeyStates[keyValue.first.first])
		{
			keyValue.second->Execute();
		}
	}

	return true;
}
