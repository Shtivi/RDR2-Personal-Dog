#include "Main.h"

void CompanionCommands::triggerFollow(RootState* state)
{
	state->currentTask = NULL;
	state->companionApi->follow();
}

void CompanionCommands::commandFollow(RootState* state)
{
	triggerFollow(state);
	const char* speech = "HORSE_FOLLOW_ME_MALE";
	if (!PED::IS_PED_MALE(state->companionDog))
	{
		speech = "HORSE_FOLLOW_ME_FEMALE";
	}
	playAmbientSpeech(player, (char*)speech);
}

void CompanionCommands::commandStay(RootState* state)
{
	state->currentTask = NULL;
	state->companionApi->stay();
	const char* speech = "HORSE_STAY_HERE_MALE";
	if (!PED::IS_PED_MALE(state->companionDog))
	{
		speech = "HORSE_STAY_HERE_FEMALE";
	}
	playAmbientSpeech(player, (char*)speech);
}

void CompanionCommands::commandPraise(RootState* state)
{
	state->calmTimer.start();
	state->companionApi->getPraised(player);
}

void CompanionCommands::commandRetrieve(RootState* state)
{
	log("retrieving lost dog");
	Vector3 spawnCoords = *getSafeCoordForPed(playerPos() + getForwardVector(player) * -10);
	if (!spawnCoords)
	{
		log("could not find proper spawn coords");
		return;
	}

	ENTITY::SET_ENTITY_COORDS(state->companionDog, spawnCoords.x, spawnCoords.y, spawnCoords.z, 0, 0, 0, 1);
	triggerFollow();
}

void CompanionCommands::commandAttack(RootState* state, Ped target)
{
	state->currentTask = NULL;
	state->companionApi->combat(target);
}
