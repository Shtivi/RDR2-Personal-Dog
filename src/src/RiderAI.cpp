#include "Main.h"

RiderAI::RiderAI(Ped ped, bool persistBlip) : PedBehavior(ped, persistBlip)
{
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);
}

bool RiderAI::canShoot()
{
	return isAlive() && isPedDriverOrShotgun(ped());
}

void RiderAI::ride(int speed, int drivingStyle)
{
	AI::TASK_CLEAR_LOOK_AT(ped());
	if (PED::IS_PED_ON_MOUNT(ped()))
	{
		Vector3 pos = entityPos(ped()) + getForwardVector(ped()) * 100;
		AI::TASK_WANDER_IN_AREA(ped(), pos.x, pos.y, pos.z, 200, 10, 10, 1);
	}
	else if (PED::IS_PED_IN_ANY_VEHICLE(ped(), 1))
	{
		Vehicle vehicle = PED::GET_VEHICLE_PED_IS_IN(ped(), false);
		Ped driver = VEHICLE::GET_PED_IN_VEHICLE_SEAT(vehicle, -1);
		AI::TASK_VEHICLE_DRIVE_WANDER(driver, vehicle, speed, drivingStyle);
	}
	else if (PED::IS_PED_ON_FOOT(ped()))
	{
		Vector3 pos = entityPos(ped());
		AI::TASK_WANDER_IN_AREA(ped(), pos.x, pos.y, pos.z, 200, 10, 10, 1);
	}
}

void RiderAI::warn(Ped suspect)
{
	AI::TASK_LOOK_AT_ENTITY(ped(), suspect, -1, 0, 0, 0);
	//playAmbientSpeech(ped(), "GENERIC_ANGRY_SHOUT");
	playAmbientSpeech(ped(), "LEAVE_NOW");
}

void RiderAI::threatenEscalated(Ped suspect)
{
	pedEquipBestWeapon(ped());
	playAmbientSpeech(ped(), "CHALLENGE_THREATEN_ARMED");
	//playAmbientSpeech(ped(), "FINAL_WARNING");
	AI::TASK_AIM_GUN_AT_ENTITY(ped(), suspect, -1, 0, 0);
}

void RiderAI::threaten(Ped enemy)
{
	pedEquipBestWeapon(ped());
	playAmbientSpeech(ped(), "CHALLENGE_THREATEN_ARMED");
	AI::TASK_AIM_GUN_AT_ENTITY(ped(), enemy, -1, 0, 0);
}

void RiderAI::fight(Ped enemy)
{
	AI::TASK_COMBAT_PED(ped(), enemy, 0, 16);
}