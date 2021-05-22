#pragma once

class BanditsCampExecutor : public AssassinationMissionExecutor
{
private:
	MapArea* area;
	Vector3 spawnLocation;
	CampingSite* campingSite;

public:
	BanditsCampExecutor(MissionData* missionData, MissionStatus status, MapArea* area);
	void update();

protected:
	virtual void prepareSet();
	virtual Ped spawnTarget();
	virtual void cleanup();
	virtual void onTargetFound();
	virtual const char* generateTrackDownMessage();
	
private:
	const char* getEnemiesModel();
};