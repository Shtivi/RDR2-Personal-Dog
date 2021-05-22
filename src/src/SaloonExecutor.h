#pragma once

class SaloonExecutor : public AssassinationMissionExecutor
{

public:
	SaloonExecutor(MissionData* missionData, MissionStatus status);
	virtual void update();

protected:
	virtual void prepareSet();
	virtual Ped spawnTarget();
	virtual const char* generateTrackDownMessage();
	bool playRandomSeatedScenario(Ped ped);
};