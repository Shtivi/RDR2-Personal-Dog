#pragma once

class BarkAsyncTask : public AsyncCompanionTask
{
private:
	int duration;
	const char* vocalization;
	Stopwatch taskTimer;
	int counter;

public:
	BarkAsyncTask(Ped ped, int duration, const char* vocalization = "BARK");
	void update();
	bool isDone();
};