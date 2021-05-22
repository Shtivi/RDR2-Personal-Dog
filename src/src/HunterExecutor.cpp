#include "Main.h"

vector<const char*> HUNTER_MODELS = {
	"a_m_m_bivroughtravellers_01",
	"a_m_m_bynroughtravellers_01",
	"a_m_m_griroughtravellers_01",
	"a_m_m_htlroughtravellers_01",
	"a_m_m_sclroughtravellers_01",
	"a_m_m_rkrroughtravellers_01",
	"a_m_m_nearoughtravellers_01",
	"a_f_m_roughtravellers_01",
	"a_f_m_roughtravellers_01",
	"a_m_m_huntertravelers_cool_01",
	"a_m_m_huntertravelers_warm_01"
};

vector<const char*> ANIMAL_TARGET_MODELS = {
	"a_c_deer_01",
	"a_c_sheep_01",
	"a_c_pronghorn_01"
};

HunterExecutor::HunterExecutor(MissionData* missionData, MissionStatus status)
	: AssassinationMissionExecutor(missionData, status)
{
	isTargetFleeing = false;

	Vector3* desiredLocation = getSafeCoordForPed(around(missionData->startPosition, rndInt(0, 35)));
	if (!desiredLocation)
	{
		spawnLocation = missionData->startPosition;
	}
	else 
	{
		spawnLocation = *desiredLocation;
	}
}

void HunterExecutor::update()
{
	AssassinationMissionExecutor::update();

	if (getObjective() == MissionObjective::ReachArea || getObjective() == MissionObjective::AssassinateTarget)
	{
		Ped hunter = getTargetPed();

		if (PED::IS_PED_FLEEING(animalTarget) && 
			distanceBetweenEntities(hunter, animalTarget) > 60 &&
			PED::_0xD5FE956C70FF370B(hunter) /*is target crouching*/)
		{
			Vector3 destination = entityPos(hunter) + getForwardVector(hunter) * 100;
			AI::CLEAR_PED_TASKS(getTargetPed(), 0, 0);
			playAmbientSpeech(getTargetPed(), "GENERIC_CURSE_MED");
			Object seq;
			AI::OPEN_SEQUENCE_TASK(&seq);
			AI::_0x17293C633C8AC019(0, false, 0, 0); // stop crouching
			AI::TASK_AIM_GUN_AT_ENTITY(0, animalTarget, 4000, 0, 0);
			AI::_0x92DB0739813C5186(0, horse, -1, -1, 1.5f, 1, 0, 0); // mount horse
			AI::TASK_WANDER_IN_AREA(0, destination.x, destination.y, destination.z, 200, 10, 10, 1);
			AI::CLOSE_SEQUENCE_TASK(seq);
			AI::TASK_PERFORM_SEQUENCE(hunter, seq);
			PED::SET_PED_KEEP_TASK(hunter, 1);
		}

		if (getObjective() == MissionObjective::AssassinateTarget && 
			!isTargetFleeing && 
			(hasPedClearLosInFront(hunter, player, 12) || distance(getTargetPed()) < 5))
		{
			isTargetFleeing = true;
			playAmbientSpeech(getTargetPed(), "GET_AWAY_FROM_ME");

			// 33554432 flee?
			Object seq;
			AI::OPEN_SEQUENCE_TASK(&seq);
			AI::_0x17293C633C8AC019(0, false, 0, 0); // stop crouching
			if (distanceBetweenEntities(hunter, horse) < 10 && !ENTITY::IS_ENTITY_DEAD(horse))
			{
				AI::_0x92DB0739813C5186(0, horse, -1, -1, 3.0f, 1, 0, 0); // mount horse
				AI::_0xFD45175A6DFD7CE9(0, player, 3, 0, -999.0f, -1, 0); // flee
			} 
			else
			{
				AI::TASK_COMBAT_PED(0, player, 0, 16);
			}
			AI::CLOSE_SEQUENCE_TASK(seq);
			AI::TASK_PERFORM_SEQUENCE(hunter, seq);
			PED::SET_PED_KEEP_TASK(hunter, 1);
		}
	}
}

bool HunterExecutor::wasTargetJustFound()
{
	return
		PLAYER::IS_PLAYER_FREE_AIMING_AT_ENTITY(PLAYER::PLAYER_ID(), getTargetPed()) ||
		hasPedClearLosInFront(player, getTargetPed(), getMissionData()->identificationDistance);
}

void HunterExecutor::prepareSet()
{
	Ped hunter = getTargetPed();

	horse = addMissionEntity(createAmbientHorse(around(spawnLocation, 8)));
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(horse, 1);

	Vector3 targetPos = around(spawnLocation, rndInt(55, 70));
	animalTarget = addMissionEntity(createPed(ANIMAL_TARGET_MODELS.at(rndInt(0, ANIMAL_TARGET_MODELS.size())), getGroundPos(targetPos)));

	Vector3 destination = spawnLocation + getForwardVector(hunter) * 100;

	// EAT task
	int unk = AI::_0x244430C13BA5258E(animalTarget, 0, 1, 0);
	AI::_0xBD7949BD07299672(animalTarget, unk, 0);

	Object seq;
	AI::OPEN_SEQUENCE_TASK(&seq);
	AI::_0x17293C633C8AC019(0, true, 0, 0); // crouch
	AI::_0x8D7F2A63688C20A4(0, getTargetPed(), 42, 2, -1);
	AI::_0x8D7F2A63688C20A4(0, animalTarget, 35, 1, -1);
	AI::TASK_AIM_GUN_AT_ENTITY(0, animalTarget, 4000, 0, 0);
	AI::TASK_SHOOT_AT_ENTITY(0, animalTarget, 10000, -957453492, 0);
	AI::_0x17293C633C8AC019(0, false, 0, 0); // crouch
	AI::_0x502EC17B1BED4BFA(0, animalTarget); // pick up
	AI::_0x6D3D87C57B3D52C7(0, animalTarget, horse, 0); // place on horse
	AI::_0x92DB0739813C5186(0, horse, -1, -1, 2.0f, 1, 0, 0); // mount horse
	AI::TASK_WANDER_IN_AREA(0, destination.x, destination.y, destination.z, 200, 10, 10, 1);
	AI::CLOSE_SEQUENCE_TASK(seq);
	AI::TASK_PERFORM_SEQUENCE(hunter, seq);
	PED::SET_PED_KEEP_TASK(hunter, 1);
}

Ped HunterExecutor::spawnTarget()
{
	Ped ped = createPed(HUNTER_MODELS.at(rndInt(0, HUNTER_MODELS.size())), spawnLocation);
	WEAPON::REMOVE_ALL_PED_WEAPONS(ped, 0, 0);
	WAIT(500);
	giveWeaponToPed(ped, WeaponHash::SniperrifleCarcano, 50, true);
	return ped;
}

const char* HunterExecutor::generateTrackDownMessage()
{
	return "Track and kill the hunter. He is somewhere around.";
}