#include "Main.h"

vector<const char*> SALOON_TARGET_MODELS = {
	"a_m_m_bivroughtravellers_01",
	"a_m_m_bynroughtravellers_01",
	"a_m_m_griroughtravellers_01",
	"a_m_m_htlroughtravellers_01",
	"a_m_m_lowersdtownfolk_01",
	"a_m_m_lowersdtownfolk_02",
	"a_m_m_valcriminals_01",
	"a_m_m_valtownfolk_01",
	"a_m_m_valtownfolk_02",
	"a_m_m_valfarmer_01"
};

vector<int> SALOON_CHAIRS = {
	-1443906703,
	 325252933,
	 1609095284,
	 -1786034615,
	 2096698905,
	 -1763848034,
	 1407600554,
	 1999055955,
	 375867283,
	 306472818,
	 1186910594,
	 -1426374160,
	 1489783104,
	 -289651159
};

vector<const char*> SALOON_SEATED_SCENARIOS = {
	"PROP_HUMAN_SEAT_CHAIR_CIGAR",
	"PROP_HUMAN_SEAT_CHAIR_SMOKING",
	"PROP_HUMAN_SEAT_CHAIR_TABLE_DRINKING_WHISKEY",
	"PROP_HUMAN_SEAT_CHAIR_TABLE_DRINKING_BADASS_BEER",
	"PROP_HUMAN_SEAT_CHAIR_TABLE_DRINKING_BADASS_WHISKEY",
	"PROP_HUMAN_SEAT_CHAIR_TABLE_DRINKING",
	"PROP_HUMAN_SEAT_CHAIR_TABLE_DRINKING_BADASS"
};

SaloonExecutor::SaloonExecutor(MissionData* missionData, MissionStatus status)
	: AssassinationMissionExecutor(missionData, status)
{
}

void SaloonExecutor::update()
{
	AssassinationMissionExecutor::update();
}

void SaloonExecutor::prepareSet()
{
	Ped targetPed = getTargetPed();
	if (!playRandomSeatedScenario(targetPed))
	{
		AI::_0x524B54361229154F(targetPed, GAMEPLAY::GET_HASH_KEY("WORLD_HUMAN_DRINKING"), -1, true, true, 0, true);
	}
}

Ped SaloonExecutor::spawnTarget()
{
	const char* targetModel = SALOON_TARGET_MODELS.at(rndInt(0, SALOON_TARGET_MODELS.size()));
	Vector3 spawnPosition = around(getMissionData()->startPosition, rndInt(2, 10), true);
	Ped targetPed = createPed(targetModel, spawnPosition);

	return targetPed;
}

const char* SaloonExecutor::generateTrackDownMessage()
{
	return "Identify and assassinate the target at the saloon";
}


bool SaloonExecutor::playRandomSeatedScenario(Ped ped)
{
	Object chair = findUnoccupiedObjectAround(entityPos(ped), 10, SALOON_CHAIRS, 1);
	if (!chair)
	{
		return false;
	}

	const char* scenarioType = SALOON_SEATED_SCENARIOS.at(rndInt(0, SALOON_SEATED_SCENARIOS.size()));
	playSeatedScenario(ped, chair, "PROP_HUMAN_SEAT_CHAIR_TABLE_DRINKING");
	return true;
}