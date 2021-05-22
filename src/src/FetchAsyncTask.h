#pragma once

class FetchAsyncTask : public AsyncCompanionTask
{
private:
	Entity entity;
	float distanceThreshold;

public:
	FetchAsyncTask(Ped ped, Entity entity, int timeout = 10000, float distanceThreshold = 1.5);
	bool isDone();
	Entity getEntity();
	AsyncCompanionTask* getChainedTask();
};