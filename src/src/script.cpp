/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#include "Main.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Ped player;

bool debugOn = false;

CompanionEngine* engine = NULL;

bool Initialize()
{
	player = PLAYER::PLAYER_PED_ID();
	initializeLogger();
	log("Dog Companion by Shtivi - 1.0.1");

	DECORATOR::DECOR_REGISTER("SH_CMP_companion", 3);
	DECORATOR::DECOR_REGISTER("SH_CMP_health_core", 3);

	ScriptSettings::load(DataFiles::getDataFilePath("DogCompanion.ini"), new SettingsMap {
		{"ShowCores", "1"},
		{"MarkHuntedAnimals", "1"},
		{"AllowAttackPrompt", "1"},
		{"AttackPromptAnimalsOnly", "0"},
		{"IgnorePlayerCombatWhenSittingDown", "1"},
		{"AllowEnteringInteriors", "0"},
		{"InvincibleDog", "0"}
	});

	if (!DataFiles::load())
	{
		return false;
	}
	engine = new CompanionEngine();
	engine->start();

	return true;
}

void main()
{
	WAIT(500);


	if (!Initialize())
	{
		showSubtitle("Failed to load Dog Companion, please check the logs for more info.");
		return;
	}

	while (true)
	{
		player = PLAYER::PLAYER_PED_ID();
		try
		{
			updateUI();
			engine->update();
		}
		catch (...)
		{
			log("Something wrong happened");
			std::exception_ptr ex = std::current_exception();
			try
			{
				if (ex)
				{
					rethrow_exception(ex);
				}
				else
				{
					log("No exception captured.");
				}
			}
			catch (const exception& e)
			{
				const char * logMsg =
					string("[DogCompanion] Fatal: ")
					.append(e.what())
					.append(", check the logs for more info.").c_str();
				
				log(logMsg);
				showSubtitle(logMsg);
			}
		}

		if (debugOn)
		{
			Vector3 pos = playerPos();

			Hash weaponHash;
			WEAPON::GET_CURRENT_PED_WEAPON(player, &weaponHash, 0, 0, 0);
			if (weaponHash != MISC::GET_HASH_KEY("WEAPON_UNARMED")) {
				Entity e;
				if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &e) /*&& distanceBetweenEntities(player, e) < 20*/) {
					if (IsKeyJustUp(VK_KEY_Z)) 
					{
						PED::RESURRECT_PED(e);
						PED::REVIVE_INJURED_PED(e);
						showSubtitle("revive");

					}
					debug(ENTITY::GET_ENTITY_HEALTH(e));

				}
				else
				{
				}
			}
			else
			{
				Entity targetEntity;
				if (PLAYER::GET_PLAYER_TARGET_ENTITY(PLAYER::PLAYER_ID(), &targetEntity))
				{
					if (IsKeyJustUp(VK_KEY_Z)) 
					{
					}
					debug(PED::_GET_PED_REMAINING_REVIVAL_TIME(targetEntity, 1));
				}
				else
				{

				}
			}

			if (PED::IS_PED_ON_MOUNT(player))
			{
				ENTITY::SET_ENTITY_INVINCIBLE(PED::GET_MOUNT(player), 1);
			}


			LAW::_CLEAR_CURRENT_PURSUIT();
			
			if (IsKeyJustUp(VK_KEY_X))
			{
			}

			if (IsKeyJustUp(VK_F1))
			{
				//TASK::CLEAR_PED_TASKS(player, 1, 1);
				Propset::spawn(playerPos() + getForwardVector(player) * 15, "pg_re_armsdeal01x");
			}


			if (IsKeyJustUp(VK_KEY_Z))
			{
				Vehicle veh = createVehicle(MISC::GET_HASH_KEY("coach2"), playerPos() + getForwardVector(player) * 10);
				//invoke<Void>(0xC0F0417A90402742, veh, MISC::GET_HASH_KEY("pg_re_coachrobbery01x"));
				//VEHICLE::_0xF489F94BFEE12BB0(veh, -1027511593); // Adds $100 loot
				//VEHICLE::_0xF489F94BFEE12BB0(veh, -1732002606); // Adds valuable loots (naval compass, coin stacks, earing, ring)
				VEHICLE::_0xF489F94BFEE12BB0(veh, 1767696941); // Adds valuable loots (naval compass, coin stacks, earing, ring)

				//setVehicleCargo(veh, (VehicleCargoHash));
				//PROPSET::_ADD_LIGHT_PROP_SET_TO_VEHICLE(veh, );
			}

			if (IsKeyJustUp(VK_F3))
			{

				Ped ped = createPed("a_c_doghusky_01", playerPos() + getForwardVector(player) * 5);

				ENTITY::SET_ENTITY_PROOFS(ped, 8 + 16, 0);

				//WAIT(1000);
				//TASK::_TASK_ANIMAL_BLEED_OUT(ped, 0, 0, WeaponHash::WeaponBow, 0, 0);
				//WAIT(5000);
				//PED::SET_PAUSE_PED_WRITHE_BLEEDOUT(ped, true);
			}

			if (IsKeyJustUp(VK_KEY_K))
			{
			}
		}

		if (false && IsKeyJustUp(VK_F2))
		{
			setDebugMode(!isDebugMode());
		}

		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}

void debug(const char* text) {
	HUD::_DISPLAY_TEXT(MISC::VAR_STRING(10, "LITERAL_STRING", text), 0, 0);
}

void debug(string text) {
	debug(text.c_str());
}

void debug(Vector3 pos) {
	stringstream str;
	str << pos.x << ", " << pos.y << ", " << pos.z;
	debug(str.str());
}

void debug(float f) {
	debug(to_string(f));
}

void debug(int n) {
	debug(to_string(n));
}

void debug(bool b) {
	debug(to_string(b));
}

void debug(Hash hash) {
	debug(to_string((int)hash));
}

void logPlayerPos()
{
	Vector3 playerPos = entityPos(player);
	float ground;
	MISC::GET_GROUND_Z_FOR_3D_COORD(playerPos.x, playerPos.y, playerPos.z, &ground, false);
	std::stringstream output;
	output << "\n"
		<< playerPos.x << ", " << playerPos.y << ", " << playerPos.z << "\n"
		<< playerPos.x << ", " << playerPos.y << ", " << ground << "\n"
		<< "heading: " << ENTITY::GET_ENTITY_HEADING(player);

	log(output.str().c_str());
	showSubtitle(to_string(getGroundPos(playerPos)));
}

bool isDebugMode()
{
	return debugOn;
}

void setDebugMode(bool toggle)
{
	debugOn = toggle;
	if (debugOn)
	{
		showSubtitle("Debug tools ON");
		ENTITY::SET_ENTITY_INVINCIBLE(player, true);
	}
	else
	{
		showSubtitle("Debug tools OFF");
		ENTITY::SET_ENTITY_INVINCIBLE(player, false);
	}
}