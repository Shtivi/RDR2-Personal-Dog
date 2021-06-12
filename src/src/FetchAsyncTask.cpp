#include "Main.h"

FetchAsyncTask::FetchAsyncTask(Ped ped, Entity entity, int timeout, float distanceThreshold)
	: AsyncCompanionTask(ped)
{
	this->entity = entity;
	this->distanceThreshold = distanceThreshold;

	Vector3 entityCoords = entityPos(entity);
	Object seq;
	TASK::OPEN_SEQUENCE_TASK(&seq);
	TASK::TASK_FOLLOW_NAV_MESH_TO_COORD(0, entityCoords.x, entityCoords.y, entityCoords.z, 3, timeout, 1, false, 0);
	TASK::TASK_LOOK_AT_ENTITY(0, player, 6000, 0, 0, 0);
	TASK::TASK_PAUSE(0, 6000);
	TASK::CLOSE_SEQUENCE_TASK(seq);
	TASK::TASK_PERFORM_SEQUENCE(ped, seq);
}

bool FetchAsyncTask::isDone()
{
	return distance(getPed(), entity) <= distanceThreshold;
}

Entity FetchAsyncTask::getEntity()
{
	return this->entity;
}

AsyncCompanionTask* FetchAsyncTask::getChainedTask()
{
	return new BarkAsyncTask(getPed(), 6);
}