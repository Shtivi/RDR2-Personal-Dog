#include "Main.h"

vector<const char*> RIDER_MODELS = {
	"a_m_m_bivroughtravellers_01",
	"a_m_m_bynroughtravellers_01",
	"a_m_m_griroughtravellers_01",
	"a_m_m_htlroughtravellers_01",
	"a_m_m_sclroughtravellers_01",
	"a_m_m_rkrroughtravellers_01",
	"a_m_m_nearoughtravellers_01",
	"a_f_m_roughtravellers_01",
	"a_f_m_roughtravellers_01"
};

RidersExecutor::RidersExecutor(MissionData* missionData, MissionStatus status, MapArea* area)
	: AssassinationMissionExecutor(missionData, status)
{
	this->area = area;
	spawnLocation = getClosestRoad(missionData->startPosition, 100);
	if (!spawnLocation)
	{
		spawnLocation = missionData->startPosition;
	}
	riders = NULL;
}


void RidersExecutor::update()
{
	AssassinationMissionExecutor::update();

	if (riders) 
	{
		riders->update();
	}
}

void RidersExecutor::prepareSet()
{
	int max = 3;
	int min = 2;

	if (getMissionData()->difficultyLevel > MissionDifficulty::Normal)
	{
		min = 3;
		max = 5;
	}

	int riders = rndInt(min, max + 1);
	for (int i = 0; i < riders; i++)
	{
		addRider();
	}
}

Ped RidersExecutor::spawnTarget()
{
	Ped horse = addMissionEntity(createAmbientHorse(spawnLocation));
	Ped targetPed = createPedOnHorse((char*)getRidersModel(), horse, -1);
	DECORATOR::DECOR_SET_INT(targetPed, "loot_money", rndInt(1200, 1600));
	WAIT(1000);
	riders = new RidersGroup(targetPed, 13, 7, true);
	return targetPed;
}

const char* RidersExecutor::generateTrackDownMessage()
{
	return "Track down and kill the bandit. He is travelling with his gang somewhere around.";
}

const char* RidersExecutor::getRidersModel()
{
	if (getMissionData()->difficultyLevel == MissionDifficulty::Normal)
	{
		return RIDER_MODELS.at(rndInt(0, RIDER_MODELS.size()));
	}
	else
	{
		return area->getRandomLocalGangPedModel();
	}
}

void RidersExecutor::addRider()
{
	Vector3 pos = entityPos(getTargetPed()) + getForwardVector(getTargetPed()) * rndInt(-8, 8);
	Ped horse = addMissionEntity(createAmbientHorse(pos));
	Ped rider = addMissionEntity(createPedOnHorse((char*)getRidersModel(), horse, -1));
	DECORATOR::DECOR_SET_INT(rider, "honor_override", 0);
	DECORATOR::DECOR_SET_INT(rider, "loot_money", rndInt(450, 700));
	riders->addRider(rider);
	WAIT(500);
}