#pragma once

namespace dae
{
	class Command;

	enum class KeyState : int
	{
		Down,
		Pressed,
		Up
	};


	class Input final : public Singleton<Input>
	{
	public:
		using ControllerKey = std::tuple<unsigned, GamePad::ControllerButton, KeyState>;
		using ControllerCommands = std::map<ControllerKey, std::shared_ptr<Command>>;
		//using KeyboardCommands = std::map<SDL_KeyCode, std::unique_ptr<Command>>;

		using KeyboardKey = std::pair <SDL_Scancode, SDL_EventType>;
		using KeyboardCommands = std::map<KeyboardKey, std::shared_ptr<Command>>;

		void AddController(std::shared_ptr<GamePad> controller) { m_Controllers.emplace_back(controller); }

		void AddCommand(const ControllerKey& key, std::shared_ptr<Command> command);
		void AddCommand(const KeyboardKey key, std::shared_ptr<Command> command);

		const ControllerCommands& GetControllerCommands() const { return m_ControllerCommands; }
		const KeyboardCommands& GetKeyboardCommands() const { return m_KeyboardCommands; }
		std::vector<std::shared_ptr<GamePad>> GetControllers() const { return m_Controllers; }

	private:
		ControllerCommands m_ControllerCommands;
		KeyboardCommands m_KeyboardCommands;
		std::vector<std::shared_ptr<GamePad>> m_Controllers;
	};
}