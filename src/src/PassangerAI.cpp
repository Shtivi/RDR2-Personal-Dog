#include "Main.h"

PassangerAI::PassangerAI(Ped ped)
	: PedBehavior(ped)
{
	isPleading = false;
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, 1);
	start();
}

void PassangerAI::update()
{
	PedBehavior::update();

	if (!this->isActive())
	{
		return;
	}

	if (hasGunPointedAt() && !isPleading)
	{
		isPleading = true;
		handsUp(player);
	}
}

void PassangerAI::handsUp(Ped cowerFrom)
{
	AI::TASK_HANDS_UP(ped(), -1, cowerFrom, 0, 0);
	playAmbientSpeech(ped(), "PLEAD");
}

void PassangerAI::cower(Ped cowerFrom)
{
	AI::TASK_COWER(ped(), -1, cowerFrom, 0);
}

bool PassangerAI::hasGunPointedAt()
{
	float distanceToPlayer = distanceBetweenEntities(ped(), player);
	Player playerId = PLAYER::PLAYER_ID();

	if (distanceToPlayer > 20)
	{
		return false;
	}

	if (PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(playerId, ped()) && hasPedClearLosInFront(ped(), player, 20))
	{
		return true;
	}

	return false;
}