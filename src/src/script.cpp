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
bool invincible = false;

CompanionEngine* engine = NULL;

bool Initialize()
{
	player = PLAYER::PLAYER_PED_ID();
	initializeLogger();
	log("Dog Companion by Shtivi - 1.0.0");

	DECORATOR::DECOR_REGISTER("SH_CMP_companion", 3);

	ScriptSettings::load(DataFiles::getDataFilePath("DogCompanion.ini"), new SettingsMap {
		{"ShowCores", "1"}
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

	showSubtitle("DogCompaion BETA by Shtivi");

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
			//debug(DECISIONEVENT::IS_SHOCKING_EVENT_IN_SPHERE(GAMEPLAY::GET_HASH_KEY("EVENT_SHOCKING_DANGEROUS_ANIMAL"), pos.x, pos.y, pos.z, 40));
			//debug((int)PED::_0xCD66FEA29400A0B5(player));

			if (PURSUIT::_0xAD401C63158ACBAA(PLAYER::PLAYER_ID()))
			{
				debug("INCIDENT");
			}

			Hash weaponHash;
			WEAPON::GET_CURRENT_PED_WEAPON(player, &weaponHash, 0, 0, 0);
			if (weaponHash != GAMEPLAY::GET_HASH_KEY("WEAPON_UNARMED")) {
				Entity e;
				if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &e) /*&& distanceBetweenEntities(player, e) < 20*/) {
					if (IsKeyJustUp(VK_KEY_Z)) 
					{

					}
					getPedSize(e);
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
					debug(targetEntity);
				}
				else
				{

				}
			}

			if (PED::IS_PED_ON_MOUNT(player))
			{
				ENTITY::SET_ENTITY_INVINCIBLE(PED::GET_MOUNT(player), 1);
			}


			PURSUIT::CLEAR_CURRENT_PURSUIT();
			//PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
			//PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), 0);

			if (IsKeyJustUp(VK_KEY_X))
			{
			}

			if (IsKeyJustUp(VK_F1))
			{
				invincible = !invincible;
				if (invincible) {
					showSubtitle("invincible ON");
				}
				else
				{
					showSubtitle("invincible OFF");

				}
			}


			if (IsKeyJustUp(VK_KEY_Z))
			{
				AI::_0x524B54361229154F(engine->getState()->companionDog, GAMEPLAY::GET_HASH_KEY("WORLD_ANIMAL_DOG_GUARD_GROWL"), 10000, 1, 1, 0, 1);
				//engine->getState()->companionApi->playVocalization("GROWL");
			}


			if (IsKeyJustUp(VK_F3))
			{

				Ped ped = createPed("a_c_doghusky_01", playerPos() + getForwardVector(player) * 5);
				ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&ped);
				//Ped ped = createPed("a_c_rabbit_01", playerPos() + getForwardVector(player) * 22);
				//PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);

				//Ped ped = createPed("a_m_m_bivroughtravellers_01", playerPos() + getForwardVector(player) * 5);
				//AI::TASK_COMBAT_PED(ped, player, 0, 16);
			}

			if (IsKeyJustUp(VK_KEY_K))
			{
				Ped ped = createPed("a_c_snakeredboa_01", playerPos() + getForwardVector(player) * 12);
				//Ped ped = createPed("a_c_wolf_small", playerPos() + getForwardVector(player) * 25);
				//Ped ped = createPed("a_m_m_bivroughtravellers_01", playerPos() + getForwardVector(player) * 12);
				PED::SET_PED_CONFIG_FLAG(ped, 6, 1);
				DECORATOR::DECOR_SET_INT(ped, "honor_override", 0);
				
				PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);
				//ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);

				//WAIT(500);
				//AI::_0x779A2FFACEFAEA7B(dog, ped, 0, 1, 3, 1, 0); // TASK_GRAPPLE
				//AI::TASK_COMBAT_PED(ped, player, 0, 16);
				//WAIT(200);
				//showSubtitle(to_string(PED::IS_PED_IN_COMBAT(ped, player)).c_str());
			}
		}

		if (true && IsKeyJustUp(VK_F2))
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
	UI::DRAW_TEXT((char*)UI::_CREATE_VAR_STRING(10, "LITERAL_STRING", text), 0, 0);
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
	GAMEPLAY::GET_GROUND_Z_FOR_3D_COORD(playerPos.x, playerPos.y, playerPos.z, &ground, false);
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