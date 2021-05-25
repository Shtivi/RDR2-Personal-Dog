#pragma once

class TrackEntityAsyncTask : public AsyncCompanionTask
{
private:
	Entity huntedEntity;
	float maxDistanceFromPlayer;

public:
	TrackEntityAsyncTask(Ped ped, Entity huntedEntity, float maxDistanceFromPlayer);
	bool isDone();
	bool isFailed();
	void update();

private:
	float calculateStoppingDistance();
	void play(Vector3 startFrom = Vector3{ 0,0,0 });
};