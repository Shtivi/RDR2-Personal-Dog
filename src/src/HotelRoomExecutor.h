#pragma once

class HotelRoomExecutor : public AssassinationMissionExecutor
{
private:
	Place* place;
	MapArea* area;
	bool wasTargetSpooked;
	Ped wifePed;

public:
	HotelRoomExecutor(MissionData* missionData, MissionStatus status, Place* place, MapArea* area);
	virtual void update();

protected:
	virtual void prepareSet();
	virtual Ped spawnTarget();
	virtual const char* generateTrackDownMessage();
};