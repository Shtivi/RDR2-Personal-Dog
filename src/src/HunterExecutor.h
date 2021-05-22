
#pragma once

class HunterExecutor : public AssassinationMissionExecutor
{
private:
	Ped animalTarget;
	Ped horse;
	Vector3 spawnLocation;
	bool isTargetFleeing;

public:
	HunterExecutor(MissionData* missionData, MissionStatus status);
	virtual void update();

protected:
	virtual bool wasTargetJustFound();
	virtual void prepareSet();
	virtual Ped spawnTarget();
	virtual const char* generateTrackDownMessage();
};