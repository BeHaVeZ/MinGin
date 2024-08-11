#pragma once
#include "Command.h"
#include "QBertCharacter.h"
#include "GameObject.h"

namespace dae
{
	class QBertMoveUpCommand final : public Command
	{
	public:
		void Execute() override { GetActor()->GetComponent<QBertCharacter>()->MoveRightUp(); }
	};

	class QBertMoveDownCommand final : public Command
	{
	public:
		void Execute() override { GetActor()->GetComponent<QBertCharacter>()->MoveLeftDown(); }
	};

	class QBertMoveLeftCommand final : public Command
	{
	public:
		void Execute() override { GetActor()->GetComponent<QBertCharacter>()->MoveLeftUp(); }
	};

	class QBertMoveRightCommand final : public Command
	{
	public:
		void Execute() override { GetActor()->GetComponent<QBertCharacter>()->MoveRightDown(); }
	};
}