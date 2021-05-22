#pragma once

class MissionDataFactory 
{
public:
	MissionDataFactory();
	MissionData* generateMission(int id, MapArea* area);
	MissionData* generateMission(int id, MapArea* area, MissionTemplateType templateType);

private:
	MissionData* generateTransportedPrisonerMission(int id, MapArea* area);
	MissionData* generateTrainStationMission(int id, MapArea* area);
	MissionData* generateBusinessManConvoyMission(int id, MapArea* area);
	MissionData* generateLawmanMission(int id, MapArea* area);
	MissionData* generateSaloonMission(int id, MapArea* area);
	MissionData* generateJailPrisonerMission(int id, MapArea* area);
	MissionData* genereateBanditsCampMission(int id, MapArea* area);
	MissionData* generateRidersMission(int id, MapArea* area);
	MissionData* generateHunterMission(int id, MapArea* area);
	MissionData* generateAmbientPedMission(int id, MapArea* area);
	MissionData* generateAnnesburgMineManagerMission(int id, MapArea* area);
	MissionData* generateAnnesburgMineGuardMission(int id, MapArea* area);
	MissionData* generateBountyHuntersRidersMission(int id, MapArea* area);
	MissionData* generateBanditsShackMission(int id, MapArea* area);
	MissionData* generateBanditHideoutMission(int id, MapArea* area);
	MissionData* generateAmbientRancherMission(int id, MapArea* area);
	MissionData* generateArmyConvoyMission(int id, MapArea* area);
	MissionData* generateMoonshinerConvoyMission(int id, MapArea* area);
	MissionData* generateOilTraderConvoyMission(int id, MapArea* area);
	MissionData* generateGreyMission(int id, MapArea* area);
	MissionData* generateBraithwaiteMission(int id, MapArea* area);
	MissionData* generateTrainTravelerMission(int id, MapArea* area);
	MissionData* generateTrainWorkerMission(int id, MapArea* area);
	MissionData* generateHotelRoomMission(int id, MapArea* area);
};