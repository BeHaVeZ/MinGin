#pragma once
#include "Command.h"
#include "QBertCharacter.h"
#include "GameObject.h"

namespace dae
{
	class QBertMoveUpCommand final : public Command
	{
	public:
		void Execute() override { GetActor()->GetComponent<QBertCharacter>()->MoveUp(); }
	};

	class QBertMoveDownCommand final : public Command
	{
	public:
		void Execute() override { GetActor()->GetComponent<QBertCharacter>()->MoveDown(); }
	};

	class QBertMoveLeftCommand final : public Command
	{
	public:
		void Execute() override { GetActor()->GetComponent<QBertCharacter>()->MoveLeft(); }
	};

	class QBertMoveRightCommand final : public Command
	{
	public:
		void Execute() override { GetActor()->GetComponent<QBertCharacter>()->MoveRight(); }
	};
}