#include "Main.h"

vector<const char*> SLEEPING_SCENARIOS = {
	"WORLD_HUMAN_SIT_FALL_ASLEEP",
	"WORLD_HUMAN_SLEEP_GROUND_ARM",
	"WORLD_HUMAN_SLEEP_GROUND_PILLOW",
	"WORLD_HUMAN_SLEEP_GROUND_PILLOW_NO_PILLOW",
	"WORLD_PLAYER_SLEEP_BEDROLL",
	"WORLD_PLAYER_SLEEP_GROUND"
};

GuardAI::GuardAI(Ped ped, int visionRange, int hearingRange, bool persistBlip)
	: PedBehavior(ped, persistBlip)
{
	this->visionRange = visionRange;
	this->hearingRange = hearingRange;
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);
}

bool GuardAI::useNearestScenarioPoint()
{
	Vector3 pedPos = entityPos(ped());
	int scearioPoint = findUnoccupiedScenarioPointAround(pedPos, 6);
	
	if (scearioPoint != -1)
	{
		useScenarioPoint(scearioPoint);
		return true;
	}
	
	return false;
	//AI::_0x322BFDEA666E2B0E(ped(), pedPos.x, pedPos.y, pedPos.z, 10, -1, 0, 0, 0, 0);
}

void GuardAI::useScenarioPoint(int scenarioPoint, bool immediately)
{
	if (immediately)
	{
		AI::_0xCCDAE6324B6A821C(ped(), scenarioPoint, 0, -1, 0, 1, 0, 0, -1082130432, 0);
	}
	else
	{
		AI::_0xCCDAE6324B6A821C(ped(), scenarioPoint, NULL, -1, 1, 0, 0, 0, 0, 0);
	}
}

bool GuardAI::isSleeping()
{
	return false;

	//for (vector<const char*>::iterator itr = SLEEPING_SCENARIOS.begin(); itr != SLEEPING_SCENARIOS.end(); itr++)
	//{
	//	int scenarioHash = GAMEPLAY::GET_HASH_KEY((char*)*itr);
	//	if (PED::_0x34D6AC1157C8226C(ped(), scenarioHash)) // _IS_PED_USING_SCENARIO_HASH
	//	{
	//		return true;
	//	}
	//}

	//return false;
}

bool GuardAI::canShoot()
{
	return isAlive() && !isSleeping();
}

bool GuardAI::hasScenarioAvailable(float distance)
{
	return false;
}

void GuardAI::search(Vector3 aroundWhere, float radius)
{
	AI::CLEAR_PED_TASKS(ped(), 0, 0);
	pedEquipBestWeapon(ped());
	
	Object seq;
	AI::OPEN_SEQUENCE_TASK(&seq);
	for (int i = 1; i <= 3; i++)
	{
		Vector3 goPos = getRandomPedPositionInRange(aroundWhere, radius);
		Vector3 aimPos = goPos + ENTITY::GET_ENTITY_FORWARD_VECTOR(ped()) * 100;
		AI::TASK_GO_TO_COORD_ANY_MEANS(0, goPos.x, goPos.y, goPos.z, 2.0f, 0, 0, 524419, -1);
		AI::TASK_AIM_GUN_AT_COORD(0, aimPos.x, aimPos.y, aimPos.z, 4000, 0, 0);
	}
	AI::CLOSE_SEQUENCE_TASK(seq);
	AI::TASK_PERFORM_SEQUENCE(ped(), seq);
}

void GuardAI::patrol(Vector3 area, float radius)
{

}

bool GuardAI::canDetect(Ped enemy)
{
	if (!canShoot()) {
		return false;
	}

	float enemyDistance = distanceBetweenEntities(enemy, ped());
	if (enemyDistance <= visionRange && hasPedClearLosInFront(ped(), enemy, visionRange)) {
		return true;
	}

	if (enemyDistance < 20 && (AI::IS_PED_RUNNING(enemy) || AI::IS_PED_SPRINTING(enemy)))
	{
		return true;
	}

	if (enemyDistance < 2 && !PED::_0xD5FE956C70FF370B(enemy)) // _GET_PED_CROUCH_MOVEMENT
	{
		return true;
	}

	return false;
}

void GuardAI::threaten(Ped enemy, bool speak)
{
	pedEquipBestWeapon(ped());
	WAIT(500);
	Object seq;
	AI::OPEN_SEQUENCE_TASK(&seq);
	AI::TASK_AIM_GUN_AT_ENTITY(0, enemy, -1, 0, 0);
	AI::CLOSE_SEQUENCE_TASK(seq);
	AI::CLEAR_PED_TASKS(ped(), 1, 1);
	AI::TASK_PERFORM_SEQUENCE(ped(), seq);

	if (speak)
	{
		playAmbientSpeech(ped(), "CHALLENGE_THREATEN_ARMED");
	}
}

void GuardAI::warn(Ped enemy)
{
	AI::CLEAR_PED_TASKS(ped(), 0, 0);

	Object seq;
	AI::OPEN_SEQUENCE_TASK(&seq);
	AI::TASK_TURN_PED_TO_FACE_ENTITY(0, enemy, -1, 0, 0, 0);
	AI::TASK_LOOK_AT_ENTITY(0, enemy, -1, 0, 0, 0);
	AI::CLOSE_SEQUENCE_TASK(seq);
	AI::TASK_PERFORM_SEQUENCE(ped(), seq);
	playAmbientSpeech(ped(), "GET_LOST");
}

bool GuardAI::isThreatenedByPlayer()
{
	return PedBehavior::isThreatenedByPlayer(visionRange, hearingRange);
}

void GuardAI::cower(Ped from)
{
	playAmbientSpeech(ped(), "PLEAD");
	AI::TASK_COWER(ped(), -1, from, 0);
}

DangerousEvent GuardAI::detectEventsAround()
{
	if (!isAlive())
	{
		return DangerousEvent();
	}

	vector<Ped> deadBodies = detectBodiesNearby();
	if (deadBodies.size())
	{
		return DangerousEvent(DangerousEvents::DeadBody, deadBodies);
	}

	Vector3 lastPlayerImpactCoords = getPedLastWeaponImpactCoord(player);
	if (distance(lastPlayerImpactCoords, ped()) < 15) {
		return DangerousEvent(DangerousEvents::ShotsHeard, entityPos(ped()) - lastPlayerImpactCoords, player);
	}
	
	return DangerousEvent();
}

void GuardAI::addIgnorableBody(Ped deadBody)
{
	ignoreableBodies.insert(deadBody);
}

void GuardAI::addIgnorableBodies(vector<Ped> deadBodies)
{
	for (vector<Ped>::iterator itr = deadBodies.begin(); itr != deadBodies.end(); itr++)
	{
		ignoreableBodies.insert(*itr);
	}
}

bool GuardAI::shouldIgnoreBody(Ped deadBody)
{
	return ignoreableBodies.find(deadBody) != ignoreableBodies.end();
}

vector<Ped> GuardAI::detectBodiesNearby()
{
	vector<Ped> deadBodies = getNearbyDeadBodies(ped(), 20, visionRange);

	vector<Ped>::iterator bodiesItr = deadBodies.begin();
	while (bodiesItr != deadBodies.end())
	{
		if (shouldIgnoreBody(*bodiesItr))
		{
			bodiesItr = deadBodies.erase(bodiesItr);
		}
		else
		{
			addIgnorableBody(*bodiesItr);
			bodiesItr++;
		}
	}
}