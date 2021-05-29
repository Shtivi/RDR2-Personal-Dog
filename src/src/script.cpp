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
	log("Dog Companion by Shtivi - BETA 1.0.0");

	DECORATOR::DECOR_REGISTER("SH_CMP_companion", 3);
	DECORATOR::DECOR_REGISTER("SH_CMP_health_core", 3);

	ScriptSettings::load(DataFiles::getDataFilePath("DogCompanion.ini"), new SettingsMap {
		{"ShowCores", "1"},
		{"MarkHuntedAnimals", "1"},
		{"AllowAttackPrompt", "1"},
		{"AttackPromptAnimalsOnly", "0"}
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
			//debug(DECISIONEVENT::IS_SHOCKING_EVENT_IN_SPHERE(GAMEPLAY::GET_HASH_KEY("EVENT_SHOCKING_DANGEROUS_ANIMAL"), pos.x, pos.y, pos.z, 40));
			//debug((int)PED::_0xCD66FEA29400A0B5(player));
			//if (PURSUIT::_0xAD401C63158ACBAA(PLAYER::PLAYER_ID()))
			//{
			//	debug("INCIDENT");
			//}

			Hash weaponHash;
			WEAPON::GET_CURRENT_PED_WEAPON(player, &weaponHash, 0, 0, 0);
			if (weaponHash != GAMEPLAY::GET_HASH_KEY("WEAPON_UNARMED")) {
				Entity e;
				if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &e) /*&& distanceBetweenEntities(player, e) < 20*/) {
					if (IsKeyJustUp(VK_KEY_Z)) 
					{
						//PURSUIT::_0xC805EB785824F712(1);
						//PURSUIT::_0x95878B13E272EF1F(e, player, 0, pos.x, pos.y, pos.z, -493957506);

					}
					debug(PED::GET_PED_SOURCE_OF_DEATH(e) == player);

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
						//PURSUIT::_0x10827B5A0AAC56A7(PLAYER::PLAYER_ID(), 0x0BADC882, targetEntity);
						//PURSUIT::_0xF0B67BAD53C35BD9(targetEntity, player, 0, pos.x, pos.y, pos.z, 0x0BADC882);

						//PURSUIT::_0x10827B5A0AAC56A7(PLAYER::PLAYER_ID(), -493957506, targetEntity);
						//AI::_0x30A768C30D385EC5(targetEntity, 0, 1, GAMEPLAY::GET_HASH_KEY("WEAPON_REVOLVER_NAVY"), 0, 0);
						//PED::_0x5AF24CA9C974E51A(targetEntity, player);
						//PURSUIT::_0xF0B67BAD53C35BD9(0, player,targetEntity, pos.x, pos.y, pos.z, -1521715232);
						PED::EXPLODE_PED_HEAD(targetEntity, WeaponHash::MeleeHatchet);
						WAIT(1000);
						PURSUIT::_0xF0B67BAD53C35BD9(targetEntity, player, targetEntity, pos.x, pos.y, pos.z, -493957506);

					}
					//debug(DECORATOR::DECOR_GET_INT(engine->getState()->companionDog, "SH_CMP_health_core"));
					//debug(PED::GET_RELATIONSHIP_BETWEEN_PEDS(player, targetEntity));
					//debug(PED::IS_PED_IN_COMBAT(player, targetEntity));d
				}
				else
				{

				}
			}

			if (PED::IS_PED_ON_MOUNT(player))
			{
				ENTITY::SET_ENTITY_INVINCIBLE(PED::GET_MOUNT(player), 1);
			}


			//PLAYER::CLEAR_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID());
			//PLAYER::SET_EVERYONE_IGNORE_PLAYER(PLAYER::PLAYER_ID(), 0);

			if (IsKeyJustUp(VK_KEY_X))
			{
				PURSUIT::CLEAR_CURRENT_PURSUIT();
			}

			if (IsKeyJustUp(VK_F1))
			{
				AI::CLEAR_PED_TASKS(player, 1, 1);
			}


			if (IsKeyJustUp(VK_KEY_Z))
			{
				//PURSUIT::_0x785177E4D57D7389(PLAYER::PLAYER_ID(), -1521715232, 0, 0, -1);
				//DECISIONEVENT::ADD_SHOCKING_EVENT_FOR_ENTITY(1498498500, player, 5, 20, 10, -1082130432, -1082130432, 1127481344, 0, 0, -1, -1);
				//PURSUIT::_0x75CBF20BA47E4F89(pos.x, pos.y, pos.z, -867256593);
				PURSUIT::_0xD7494DED50C6EF52(PLAYER::PLAYER_ID(), 1685924442, 2);
				//PURSUIT::_0xEDFC6C1FD1C964F5(PLAYER::PLAYER_ID(), -493957506, 0, 0, 0, 0, 0, 0, 0);
				//PURSUIT::_0xEDFC6C1FD1C964F5(PLAYER::PLAYER_ID(), -1521715232, 0, 75, 100, 0, 75, 100, 0);
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
				Ped ped = createPed("a_m_m_bivroughtravellers_01", playerPos() + getForwardVector(player) * 12);
				//Ped ped = createPed("a_c_wolf_small", playerPos() + getForwardVector(player) * 25);
				//Ped ped = createPed("a_m_m_bivroughtravellers_01", playerPos() + getForwardVector(player) * 12);
				PED::SET_PED_CONFIG_FLAG(ped, 6, 1);
				//DECORATOR::DECOR_SET_INT(ped, "honor_override", 0);

				//ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&ped);

				//WAIT(500);
				//PED::EXPLODE_PED_HEAD(ped, WeaponHash::ShotgunDoublebarrelExotic);
				WAIT(500);

				PURSUIT::_0x987BE590FB9D41E5(1);
				PURSUIT::_0xDCF12B89624AAC96(1);
				PURSUIT::_0xD7494DED50C6EF52(PLAYER::PLAYER_ID(), 0x0BADC882, 2);
				PURSUIT::_0xF0B67BAD53C35BD9(ped, player, ped, pos.x, pos.y, pos.z, 0x0BADC882);

				//Vector3 pedPos = entityPos(ped);
				//
				//struct {
				//	alignas(8) int player;
				//	alignas(8) int f_1;
				//	alignas(8) int crime;
				//	alignas(8) float x;
				//	alignas(8) float y;
				//	alignas(8) float z;
				//	alignas(8) int f_6;
				//	alignas(8) int f_7;
				//	alignas(8) int f_8;
				//	alignas(8) int f_9;
				//	alignas(8) int f_10;
				//	alignas(8) int f_11;
				//	alignas(8) int f_12;
				//	alignas(8) int f_13;
				//	alignas(8) int f_14;
				//} data = {
				//	player,
				//	0,
				//	-493957506,
				//	pedPos.x,
				//	pedPos.y,
				//	pedPos.z,
				//	0,
				//	1000,
				//	1,
				//	0,
				//	0,
				//	0,
				//	0,
				//	0,
				//	1034599207,
				//};
				//PURSUIT::_0x018F30D762E62DF8(ped, (Any*)&data);

				//DECISIONEVENT::ADD_SHOCKING_EVENT_FOR_ENTITY(869302489, ped, 0, -1, -1, -1082130432, -1082130432, 1127481344, 0, 0, -1, -1);

				//PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);
				//ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
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