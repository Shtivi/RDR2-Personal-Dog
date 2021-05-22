#include "Main.h"

Entity getPlayerTargetEntity()
{
	Entity targetEntity;
	if (PLAYER::GET_PLAYER_TARGET_ENTITY(PLAYER::PLAYER_ID(), &targetEntity))
	{
		return targetEntity;
	}

	return NULL;
}

MapAreas getPlayerMapArea()
{
	return getMapArea(player);
}

bool isPlayerUsingCampfire()
{
	// fire camp: 1020517461
	// fire camp: -1767895052
	// fire camp: 1984305068

	int playerScenarioPointType = (int)AI::_0x2D0571BB55879DA2(player); //  _GET_SCENARIO_POINT_TYPE_PED_IS_USING
	return playerScenarioPointType == -1767895052 || playerScenarioPointType == 1020517461 || playerScenarioPointType == 1984305068;
}

bool isPlayerResting()
{
	// fire camp: 1020517461
	// fire camp: -1767895052
	// fire camp: 1984305068
	// resting: -1241981548

	int playerScenarioPointType = (int)AI::_0x2D0571BB55879DA2(player); //  _GET_SCENARIO_POINT_TYPE_PED_IS_USING
	return playerScenarioPointType == -1241981548;
}

void setPlayerCoreValue(AttributeCores core, int value)
{
	ATTRIBUTE::_0xC6258F41D86676E0(player, (int)core, value);
}

int getPlayerCoreValue(AttributeCores core)
{
	return ATTRIBUTE::_0x36731AC041289BB1(player, (int)core);
}

void addToPlayerCore(AttributeCores core, int amount)
{
	setPlayerCoreValue(core, getPlayerCoreValue(core) + amount);
}

Ped getPlayerSaddleHorse()
{
	return PLAYER::_0xB48050D326E9A2F3(PLAYER::PLAYER_ID()); // _GET_SADDLE_HORSE_FOR_PLAYER
}