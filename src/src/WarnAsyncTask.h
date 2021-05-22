#pragma once

class WarnAsyncTask : public AsyncCompanionTask
{
private:
	Ped danger;
	int duration;

public:
	WarnAsyncTask(Ped ped, Ped warnFrom, int duration, const char* scenarioName = "WORLD_ANIMAL_DOG_BARK_GROWL");
	bool isDone();
};