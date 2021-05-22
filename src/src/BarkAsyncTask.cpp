#include "Main.h"

BarkAsyncTask::BarkAsyncTask(Ped ped, int duration, const char* vocalization)
	: AsyncCompanionTask(ped)
{
	this->duration = duration;
	this->vocalization = vocalization;
	this->counter = 0;

	this->taskTimer.start();
}

bool BarkAsyncTask::isDone()
{
	return this->taskTimer.getElapsedSeconds() >= this->duration;
}

void BarkAsyncTask::update()
{
	if (!AUDIO::_0xC265DF9FB44A9FBD(getPed()))
	{
		if ((int)taskTimer.getElapsedSeconds() % 2 == 1)
		{
			return;
		}
		invoke<Void>(0xEE066C7006C49C0A, getPed(), vocalization, 1);
		counter++;
	}
}