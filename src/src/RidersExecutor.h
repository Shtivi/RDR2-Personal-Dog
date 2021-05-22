#pragma once

class RidersExecutor : public AssassinationMissionExecutor
{
private:
	Vector3 spawnLocation;
	RidersGroup* riders;
	MapArea* area;

public:
	RidersExecutor(MissionData* missionData, MissionStatus status, MapArea* area);
	virtual void update();

protected:
	virtual void prepareSet();
	virtual Ped spawnTarget();
	virtual const char* generateTrackDownMessage();

private:
	const char* getRidersModel();
	void addRider();
};