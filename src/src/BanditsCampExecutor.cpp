#include "Main.h"

BanditsCampExecutor::BanditsCampExecutor(MissionData* missionData, MissionStatus status, MapArea* area)
	: AssassinationMissionExecutor(missionData, status)
{
	this->area = area;
	Vector3* safeCoord = getSafeCoordForPed(missionData->startPosition);
	if (safeCoord)
	{
		spawnLocation = *safeCoord;
	}
	else
	{
		spawnLocation = missionData->startPosition;
	}

	campingSite = NULL;
}

void BanditsCampExecutor::update()
{
	AssassinationMissionExecutor::update();

	if (campingSite)
	{
		campingSite->update();
	}

	if (getObjective() == MissionObjective::AssassinateTarget)
	{
		campingSite->showBlips();
	}
}

void BanditsCampExecutor::prepareSet()
{
	int guards = 3;

	if (getMissionData()->difficultyLevel > MissionDifficulty::Normal)
	{
		guards += rndInt(1, 3);
	}

	while (guards > 0)
	{
		campingSite->addGuard(addMissionEntity(createPed(getEnemiesModel(), around(spawnLocation, 4))));
		guards--;
	}

	WAIT(1000);
	campingSite->play();
}

Ped BanditsCampExecutor::spawnTarget()
{
	campingSite = new CampingSite(spawnLocation, 20, 8);

	Ped targetPed = createPed(getEnemiesModel(), around(spawnLocation, 4));
	campingSite->addGuard(targetPed, true);
	return targetPed;
}

void BanditsCampExecutor::cleanup()
{
	AssassinationMissionExecutor::cleanup();

	if (campingSite)
	{
		campingSite->cleanup();
	}
}

void BanditsCampExecutor::onTargetFound()
{
	AssassinationMissionExecutor::onTargetFound();
	campingSite->showBlips();
}

const char* BanditsCampExecutor::generateTrackDownMessage()
{
	return "Find and eliminate the target, it is camping somewhere nearby";
}

const char* BanditsCampExecutor::getEnemiesModel()
{
	if (getMissionData()->difficultyLevel == MissionDifficulty::Normal)
	{
		return GENERIC_OUTLAWS_MODELS.at(rndInt(0, GENERIC_OUTLAWS_MODELS.size()));
	} 
	else 
	{
		return area->getRandomLocalGangPedModel();
	}
}