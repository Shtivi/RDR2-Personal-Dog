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
		{"AllowEnteringInteriors", "0"}
	});

	if (!DataFiles::load())
	{
		return false;
	}
	engine = new CompanionEngine();
	engine->start();

	return true;
}

Prompt* revive;

void main()
{
	WAIT(500);


	revive = new Prompt("Revive", GAMEPLAY::GET_HASH_KEY("INPUT_REVIVE"), PromptMode::Standard);
	revive->hide();


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

			Ped closest = getClosestPed(player);
			if (isPedADog(closest))
			{
				revive->show();
				if (revive->isActivatedByPlayer())
				{
					if (PED::_0xEBE89623EB861271(closest, 1) > 0)
					{
						debug(PED::_0xEBE89623EB861271(closest, 1));
						//revive->setTargetEntity(closest);
						//AI::_0xCD181A959CFDD7F4(player, closest, GAMEPLAY::GET_HASH_KEY("Interaction_Injection_Quick"), GAMEPLAY::GET_HASH_KEY("p_cs_syringe01x"), 0);
						AI::_0x356088527D9EBAAD(player, closest, 0);
					}
					else 
					{
						AI::_0xCD181A959CFDD7F4(player, closest, GAMEPLAY::GET_HASH_KEY("Interaction_Food"), GAMEPLAY::GET_HASH_KEY("p_brushHorse02x"), 0);

					}
				}
			}
			else
			{
				revive->hide();
			}

			Hash weaponHash;
			WEAPON::GET_CURRENT_PED_WEAPON(player, &weaponHash, 0, 0, 0);
			if (weaponHash != GAMEPLAY::GET_HASH_KEY("WEAPON_UNARMED")) {
				Entity e;
				if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &e) /*&& distanceBetweenEntities(player, e) < 20*/) {
					if (IsKeyJustUp(VK_KEY_Z)) 
					{
					}
					//debug(PED::_0xEBE89623EB861271(e, 1));
					debug(PED::_0xB655DB7582AEC805(e));
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
						showSubtitle(to_string(DECORATOR::DECOR_GET_INT(targetEntity, "metaped_outfit_request_name")));

					}
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
			
			if (IsKeyJustUp(VK_KEY_X))
			{
			}

			if (IsKeyJustUp(VK_F1))
			{
				AI::CLEAR_PED_TASKS(player, 1, 1);
			}


			if (IsKeyJustUp(VK_KEY_Z))
			{
				/*Vehicle veh = createVehicle(VehicleHash::Cart06, playerPos() + getForwardVector(player) * 12);
				VEHICLE::SET_VEHICLE_DOOR_OPEN(veh, 5, 0, 1);
				Vector3 vehPos = entityPos(veh);
				Ped ped = createPed("a_c_doghusky_01", entityPos(veh) + getForwardVector(veh) * -2);
				WAIT(1000);*/
				Ped ped = createPed("a_c_doghusky_01", entityPos(player) + getForwardVector(player) * 2);
				//AI::TASK_ENTER_VEHICLE(ped, veh, -1, 3, 1, 0, 0);
				WAIT(1000);
				//AI::_0x30A768C30D385EC5(ped, 0, 1, WeaponHash::WeaponBow, 1000, 0);
				AI::_0x8C038A39C4A4B6D6(ped, 0, 0);
			}


			if (IsKeyJustUp(VK_F3))
			{

				Ped ped = createPed("a_c_doghusky_01", playerPos() + getForwardVector(player) * 5);
				//PED::_0x77FF8D35EEC6BBC4(ped, 1, 0);
				ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&ped);
				//Ped ped = createPed("a_c_rabbit_01", playerPos() + getForwardVector(player) * 22);
				//PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);

				//Ped ped = createPed("a_m_m_bivroughtravellers_01", playerPos() + getForwardVector(player) * 5);
				//AI::TASK_COMBAT_PED(ped, player, 0, 16);
			}

			if (IsKeyJustUp(VK_KEY_K))
			{
				Ped ped = createPed("a_m_m_bivroughtravellers_01", playerPos() + getForwardVector(player) * 12);
				//PED::_0x5240864E847C691C(ped, 1);
				PED::_0x39ED303390DDEAC7(ped, 1, 1, 10000, 75);
				//Ped ped = createPed("a_c_wolf_small", playerPos() + getForwardVector(player) * 25);
				//Ped ped = createPed("a_m_m_bivroughtravellers_01", playerPos() + getForwardVector(player) * 12);
				PED::SET_PED_CONFIG_FLAG(ped, 6, 1);
				DECORATOR::DECOR_SET_INT(ped, "honor_override", 0);

				ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&ped);
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