#include "Main.h"

WarnAsyncTask::WarnAsyncTask(Ped ped, Ped warnFrom, int duration, const char* scenarioName)
	: AsyncCompanionTask(ped)
{
	this->danger = warnFrom;
	this->duration = duration;

	AI::TASK_LOOK_AT_ENTITY(ped, warnFrom, -1, 0, 0, 0);
	Object seq;
	AI::OPEN_SEQUENCE_TASK(&seq);
	AI::TASK_TURN_PED_TO_FACE_ENTITY(0, warnFrom, 1000, 0, 0, 0);
	AI::_0x524B54361229154F(0, GAMEPLAY::GET_HASH_KEY((char*)scenarioName), duration, 1, 1, 0, 1);
	AI::CLOSE_SEQUENCE_TASK(seq);
	AI::TASK_PERFORM_SEQUENCE(getPed(), seq);
}

bool WarnAsyncTask::isDone()
{
	if (AI::GET_SEQUENCE_PROGRESS(getPed()) == -1 || ENTITY::IS_ENTITY_DEAD(danger) || distance(getPed(), player) > 10)
	{
		AI::CLEAR_PED_TASKS(getPed(), 1, 1);
		AI::TASK_CLEAR_LOOK_AT(getPed());
		return true;
	}

	return false;
}