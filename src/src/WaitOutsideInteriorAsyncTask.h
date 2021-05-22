#pragma once

class WaitOutsideInteriorAsyncTask : public AsyncCompanionTask
{
private:
	Ped waitFor;

public:
	WaitOutsideInteriorAsyncTask(Ped ped, Ped waitFor);
	bool isDone();
	AsyncCompanionTask* getChainedTask();
};