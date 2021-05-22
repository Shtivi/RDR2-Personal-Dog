#include "Main.h"

MissionData::MissionData(MapArea* area)
{
	this->mapArea = area;
	this->optionalObjectives = NULL;
}

MissionTemplate* MissionData::getTemplate()
{
	return MissionTemplate::fromType(templateType);
}

int MissionData::getReward()
{
	int baseReward = getTemplate()->baseReward;
	int difficulty = (int)difficultyLevel - 1;
	int difficultyBonus = ceil((baseReward * 7.5 * difficulty) / 100);

	int distanceBonus = 0;
	float missionDistance = distance(startPosition, mapArea->getTownCoords());
	if (missionDistance > mapArea->getTownRadius() * 5)
	{
		distanceBonus = ceil(baseReward * 10 * (missionDistance / (mapArea->getTownRadius() * 4)) / 100);
	}

	return baseReward + difficultyBonus + distanceBonus;
}