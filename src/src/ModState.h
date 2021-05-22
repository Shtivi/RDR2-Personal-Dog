#pragma once

class ModState
{
private:
	const char* dataFile;
	bool autosaveEnabled;
	bool hasChanges;
	int totalMissions;
	int pendingReward;
	map<int, MissionStatus> missionStatuses;
	map<int, MissionData*> missions;
	MapAreasFactory* mapAreas;

public:
	ModState(const char* dataFile, MapAreasFactory* mapAreas, bool autosave = true);
	void save(bool force = false);
	void completeMission(int id, int reward);
	void acceptMission(int id);
	void createMission(MissionData* missionData);
	void abortMission(int id);
	int getPendingReward();
	int addReward(int reward);
	void clearReward();
	bool doesMissionExist(int id);
	MissionData* getMission(int id);
	MissionStatus getMissionStatus(int id);
	vector<int> listActiveMissions();
	int getTotalMissions();
	int countActiveMissionsInArea(MapAreas area);

private:
	void setPendingReward(int reward);
	void setMissionStatus(int id, MissionStatus status);
	void load();
	void autosave();
	void handleMissionInput(string inputLine);
	const char* serializeMission(MissionData* missionData, MissionStatus status);
	vector<OptionalObjectives>* parseOptionalObjectives(string input);
	string* serializeOptionalObjectives(vector<OptionalObjectives>* optionalObjectives);
};