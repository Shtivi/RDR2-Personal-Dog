#pragma once

class AsyncCompanionTask
{
private:
	Ped ped;

public:
	AsyncCompanionTask(Ped ped);
	Ped getPed();
	virtual void update();
	virtual bool isDone() = 0;
	virtual bool isFailed();
	virtual bool isRunning();
	virtual AsyncCompanionTask* getChainedTask();
};

#include "FetchAsyncTask.h"
#include "BarkAsyncTask.h"
#include "WarnAsyncTask.h"
#include "WaitOutsideInteriorAsyncTask.h"
#include "TrackEntityAsyncTask.h"