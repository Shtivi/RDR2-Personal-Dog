#pragma once

class ModProgress
{
private:
	bool hasChanges;
	const char* dataFile;
	std::map<int, MissionStatus> cache;

public:
	ModProgress(const char* dataFile);

	MissionStatus getMissionProgress(int missionId);
	void completeMission(int missionId);
	void startMission(int missionId);
	void enableMission(int missionId);
	void save();

private:
	void setMissionProgress(int missionId, MissionStatus status);
	void readFromDataFile();
	void parseToCache(std::string inputLine);
};