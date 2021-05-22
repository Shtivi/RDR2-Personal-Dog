#include "Main.h"

Entity lassoTarget = 0;
bool isLassoEquiped = false;
bool wasHoldingLastFrame = true;

void updateLasso()
{
	Player playerId = PLAYER::PLAYER_ID();
	Ped player = PLAYER::PLAYER_PED_ID();
	Entity target;
	Hash playerCurrWeapon;
	Hash aimControl = GAMEPLAY::GET_HASH_KEY("INPUT_AIM");

	if (WEAPON::GET_CURRENT_PED_WEAPON(player, &playerCurrWeapon, true, 0, false) &&
		playerCurrWeapon == GAMEPLAY::GET_HASH_KEY("WEAPON_LASSO"))
	{
		lassoTarget = PED::_0xB65A4DAB460A19BD(PLAYER::PLAYER_PED_ID());
	}
	else
	{
		isLassoEquiped = false;
		lassoTarget = 0;
	}
}