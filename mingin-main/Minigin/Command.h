#pragma once

#include "ScoreComponent.h"
#include "HealthComponent.h"

namespace dae
{
	//+--------------------------------------------------------------------------
	class Command
	{
	protected:
		std::shared_ptr<GameObject> GetActor() const { return m_Actor; }
	public:
		Command() = default;
		virtual ~Command() = default;
		virtual void Execute() = 0;
		void SetActor(std::shared_ptr<GameObject> actor) { m_Actor = actor; }
		void SetCommandExecuted(bool executed) { m_CommandExecuted = executed; }
		bool GetCommandExecuted() const { return m_CommandExecuted; }

		Command(const Command& other) = delete;
		Command(Command&& other) = delete;
		Command& operator=(const Command& other) = delete;
		Command& operator=(Command&& other) = delete;

	private:
		bool m_CommandExecuted;
		std::shared_ptr<dae::GameObject> m_Actor;
	};

	//+--------------------------------------------------------------------------

	class GameObject;
	class MoveCommand final : public Command
	{
	public:
		MoveCommand(std::shared_ptr<GameObject> go, const glm::vec3& dir, float speed) : m_GameObject(go), m_Direction(dir), m_Speed(speed) {};
		virtual void Execute() override;
		void SetSpeed(float speed) { m_Speed = speed; }

	private:
		std::shared_ptr<GameObject> m_GameObject;
		glm::vec3 m_Direction;
		float m_Speed;
	};

	//+--------------------------------------------------------------------------

	class KillCommand final : public Command
	{
	public:
		virtual void Execute() override { std::cout << "DAMAGED" << "\n"; }
	};

	//+--------------------------------------------------------------------------
	class AddScoreCommand final : public Command
	{
	public:
		virtual void Execute() override { GetActor()->GetComponent<ScoreComponent>()->AddScore(50); }
	};
}