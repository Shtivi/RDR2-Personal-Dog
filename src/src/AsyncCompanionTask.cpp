#include "Main.h"

AsyncCompanionTask::AsyncCompanionTask(Ped ped)
{
	this->ped = ped;
}

Ped AsyncCompanionTask::getPed()
{
	return ped;
}

bool AsyncCompanionTask::isRunning()
{
	return !isDone() && !isFailed();
}

AsyncCompanionTask* AsyncCompanionTask::getChainedTask()
{
	return NULL;
}

bool AsyncCompanionTask::isFailed()
{
	return false;
}

void AsyncCompanionTask::update()
{

}