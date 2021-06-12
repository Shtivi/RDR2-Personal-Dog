#include "Main.h"

WarnAsyncTask::WarnAsyncTask(Ped ped, Ped warnFrom, int duration, const char* scenarioName)
	: AsyncCompanionTask(ped)
{
	this->danger = warnFrom;
	this->duration = duration;

	TASK::TASK_LOOK_AT_ENTITY(ped, warnFrom, -1, 0, 0, 0);
	Object seq;
	TASK::OPEN_SEQUENCE_TASK(&seq);
	TASK::TASK_TURN_PED_TO_FACE_ENTITY(0, warnFrom, 1000, 0, 0, 0);
	TASK::TASK_START_SCENARIO_IN_PLACE_HASH(0, MISC::GET_HASH_KEY((char*)scenarioName), duration, 1, 1, 0, 1);
	TASK::CLOSE_SEQUENCE_TASK(seq);
	TASK::TASK_PERFORM_SEQUENCE(getPed(), seq);
}

bool WarnAsyncTask::isDone()
{
	if (TASK::GET_SEQUENCE_PROGRESS(getPed()) == -1 || ENTITY::IS_ENTITY_DEAD(danger) || distance(getPed(), player) > 10)
	{
		TASK::CLEAR_PED_TASKS(getPed(), 1, 1);
		TASK::TASK_CLEAR_LOOK_AT(getPed());
		return true;
	}

	return false;
}