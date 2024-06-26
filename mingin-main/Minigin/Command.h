#pragma once

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

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

	class HealthComponent;
	class KillCommand final : public Command
	{
	public:
		KillCommand(std::shared_ptr<HealthComponent> healthComponent) :
			m_pHealthComponent(healthComponent) {};
		virtual void Execute() override;

	private:
		std::shared_ptr<HealthComponent> m_pHealthComponent;
	};

	class ScoreComponent;
	class AddScoreCommand final : public Command
	{
	public:
		AddScoreCommand(std::shared_ptr<ScoreComponent> scoreComponent)
			: m_pScoreComponent(scoreComponent) {};


		virtual void Execute() override;

	private:
		std::shared_ptr<ScoreComponent> m_pScoreComponent;
	};

	class SoundSystem;
	class PlaySoundCommand final : public Command
	{
	public:
		PlaySoundCommand(SoundSystem& sl) :m_SL(sl) {};


		virtual void Execute() override;

	private:
		SoundSystem& m_SL;
	};

	class MuteSoundCommand final : public Command
	{
	public:
		MuteSoundCommand(SoundSystem& sl) :m_SL(sl) {};


		virtual void Execute() override;

	private:
		SoundSystem& m_SL;
	};

}