#include "Main.h"

vector<const char*> FANCY_PED_MODELS = {
	"a_m_m_gamhighsociety_01",
	"a_m_m_grifancytravellers_01",
	"a_m_m_htlfancytravellers_01",
	"a_m_m_strfancytourist_01",
	"a_m_m_blwupperclass_01",
	"a_m_m_nbxupperclass_01",
	"a_m_m_rhdupperclass_01",
	"a_m_m_uppertrainpassengers_01"
};

vector<const char*> WIFE_PED_MODELS = {
	"a_f_m_gamhighsociety_01",
	"a_f_m_grifancytravellers_01",
	"a_f_m_htlfancytravellers_01",
	"a_f_m_bivfancytravellers_01",
	"a_f_m_rkrfancytravellers_01",
	"a_f_m_sclfancytravellers_01"
};

HotelRoomExecutor::HotelRoomExecutor(MissionData* missionData, MissionStatus status, Place* place, MapArea* area)
	: AssassinationMissionExecutor(missionData, status)
{
	this->place = place;
	this->area = area;
	this->wasTargetSpooked = false;
	this->wifePed = NULL;
}

void HotelRoomExecutor::update()
{
	AssassinationMissionExecutor::update();

	if (getStatus() == MissionStatus::InProgress)
	{
		if (!wasTargetSpooked &&
			distance(playerPos(), place->getCoords()) < place->getRadius() &&
			hasPedClearLosInFront(getTargetPed(), player, 35))
		{
			PED::_0xFE07FF6495D52E2A(getTargetPed(), 0, 0, 0);
			PED::_0xFE07FF6495D52E2A(wifePed, 0, 0, 0);

			pedEquipBestWeapon(getTargetPed());
			AI::TASK_COMBAT_PED(getTargetPed(), player, 0, 16);

			playAmbientSpeech(wifePed, "SCARED_HELP");
			AI::_0xFD45175A6DFD7CE9(wifePed, player, 3, 0, -999.0f, -1, 0);
			wasTargetSpooked = true;
		}
	}
}

void HotelRoomExecutor::prepareSet()
{
	place->populate();

	const char* wifeModel = WIFE_PED_MODELS.at(rndInt(0, WIFE_PED_MODELS.size()));
	wifePed = addMissionEntity(createPed(wifeModel, place->getCoords()));

	vector<int> scenarioPoints = place->listScenarioPointIds();
	AI::_0xCCDAE6324B6A821C(wifePed, scenarioPoints[0], 0, -1, 0, 1, 0, 0, -1082130432, 0);
	PED::SET_PED_KEEP_TASK(wifePed, true);

	AI::_0xCCDAE6324B6A821C(getTargetPed(), scenarioPoints[1], 0, -1, 0, 1, 0, 0, -1082130432, 0);
	PED::SET_PED_KEEP_TASK(getTargetPed(), true);
}

Ped HotelRoomExecutor::spawnTarget()
{
	const char* targetModel = FANCY_PED_MODELS.at(rndInt(0, FANCY_PED_MODELS.size()));
	Ped targetPed = createPed(targetModel, place->getCoords());
	giveWeaponToPed(targetPed, WeaponHash::RevolverCattleman, 100, false);

	return targetPed;
}

const char* HotelRoomExecutor::generateTrackDownMessage()
{
	return "Find and assassinate the target";
}