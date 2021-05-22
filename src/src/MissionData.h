#pragma once

class MissionData
{
private:
	MapArea* mapArea;

public:
	int id;
	MissionTemplateType templateType;
	MapAreas area;
	Vector3 startPosition;
	int areaRadius;
	int identificationDistance;
	MissionDifficulty difficultyLevel;
	vector<OptionalObjectives>* optionalObjectives;

	MissionData::MissionData(MapArea* area);
	MissionTemplate* getTemplate();
	int getReward();
};