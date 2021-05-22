#include "Main.h"

WaitOutsideInteriorAsyncTask::WaitOutsideInteriorAsyncTask(Ped ped, Ped waitFor)
	: AsyncCompanionTask(ped)
{
	this->waitFor = waitFor;
	AI::_0x524B54361229154F(ped, GAMEPLAY::GET_HASH_KEY("WORLD_ANIMAL_DOG_SITTING"), -1, 1, 1, 0, 1);
}

bool WaitOutsideInteriorAsyncTask::isDone()
{
	return INTERIOR::GET_INTERIOR_FROM_ENTITY(waitFor) == 0;
}

AsyncCompanionTask* WaitOutsideInteriorAsyncTask::getChainedTask()
{
	AI::CLEAR_PED_TASKS(getPed(), 1, 1);
	return NULL;
}