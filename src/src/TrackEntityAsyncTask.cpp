#include "Main.h"

TrackEntityAsyncTask::TrackEntityAsyncTask(Ped ped, Entity hunted, float maxDistanceFromPlayer)
	: AsyncCompanionTask(ped)
{
	this->huntedEntity = hunted;
	this->maxDistanceFromPlayer = maxDistanceFromPlayer;

	play(playerPos());
}

bool TrackEntityAsyncTask::isDone()
{
	return distance(getPed(), huntedEntity) < calculateStoppingDistance();
}

bool TrackEntityAsyncTask::isFailed()
{
	if (ENTITY::IS_ENTITY_DEAD(huntedEntity) || distance(getPed(), player) >= maxDistanceFromPlayer)
	{
		TASK::CLEAR_PED_TASKS_IMMEDIATELY(getPed(), 1, 1);
		return true;
	}

	return false;
}

void TrackEntityAsyncTask::update()
{
	if (TASK::GET_SEQUENCE_PROGRESS(getPed()) == -1 && !isDone())
	{
		play();
	}
}

void TrackEntityAsyncTask::play(Vector3 startFrom)
{
	Vector3 entityCoords = entityPos(huntedEntity);
	Vector3 trackerCoords = entityPos(getPed());
	float optimalStoppingDistance = calculateStoppingDistance();

	Object seq;
	TASK::OPEN_SEQUENCE_TASK(&seq);

	float targetDistance = distance(entityCoords, trackerCoords);
	if (targetDistance <= optimalStoppingDistance * 1.2)
	{
		TASK::TASK_TURN_PED_TO_FACE_ENTITY(0, huntedEntity, 2000, 0, 0, 0);
		TASK::TASK_LOOK_AT_ENTITY(0, huntedEntity, -1, 0, 0, 0);
	}
	else
	{
		Vector3 direction = normalOf(entityCoords - trackerCoords);
		Vector3 destination = trackerCoords + direction * min(32, targetDistance - optimalStoppingDistance);
		TASK::TASK_START_SCENARIO_IN_PLACE_HASH(0, MISC::GET_HASH_KEY("WORLD_ANIMAL_DOG_SNIFFING_GROUND"), 2000, 1, 1, 0, 1);
		TASK::TASK_TURN_PED_TO_FACE_COORD(0, destination.x, destination.y, destination.z, 1500);
		TASK::TASK_FOLLOW_NAV_MESH_TO_COORD(0, destination.x, destination.y, destination.z, 1.5, 12000, 1, false, 0);
	}

	TASK::CLOSE_SEQUENCE_TASK(seq);
	TASK::TASK_PERFORM_SEQUENCE(getPed(), seq);
}

float TrackEntityAsyncTask::calculateStoppingDistance()
{
	return 15;
}