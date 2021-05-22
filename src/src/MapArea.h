#pragma once

#include "WorldPlaces.h"

vector<const char*> const TOWN_PED_MODELS = {
	"a_f_m_lowersdtownfolk_01",
	"a_f_m_lagtownfolk_01",
	"a_f_m_lowersdtownfolk_02",
	"a_f_m_lowersdtownfolk_03",
	"a_m_m_rhdtownfolk_01",
	"a_m_m_rhdtownfolk_02",
	"a_m_m_strtownfolk_01",
	"a_m_m_valtownfolk_01",
	"a_m_m_valtownfolk_02",
	"a_f_m_valtownfolk_01",
	"a_f_m_strtownfolk_01",
	"a_f_m_blwtownfolk_01",
	"a_f_m_blwtownfolk_02",
	"a_f_m_blwupperclass_01",
	"a_f_o_blwupperclass_01",
	"a_f_m_blwtownfolk_01",
	"a_f_m_blwtownfolk_02",
	"a_f_m_blwupperclass_01"
};

vector<const char*> const ST_DENIS_PED_MODELS = {
	"a_f_m_middlesdtownfolk_03",
	"a_f_m_middlesdtownfolk_02",
	"a_f_m_middlesdtownfolk_01",
	"a_f_o_sdupperclass_01",
	"a_f_m_lowersdtownfolk_02",
	"a_f_m_lowersdtownfolk_03",
	"a_f_m_lowersdtownfolk_01"
};

class MapArea
{
private:
	MapAreas id;
	Vector3 townCoords;
	float townRadius;
	tm lastEncounterTime;
	const char* displayName;

	vector<pair<Vector3, float>> questGiversLocations;
	vector<MissionTemplateType> missionTemplates;
	vector<Vector3> wildernessLocations;
	vector<const char*> pedModels;
	vector<Vector3> saloonLocations;
	vector<Vector3> postOfficeLocations;
	vector<pair<Vector3, float>> jailCellLocations;
	vector<Vector3> wildrenessCampingLocations;
	vector<Vector3> riderRoadLocations;
	vector<const char*> localGangModels;
	vector<Place*> places;
	vector<pair<Vector3, bool>> trainTrackLocations;

public:
	MapArea(MapAreas areaId, vector<const char*> pedModels, const char* displayName, vector<Place*> places);
	
	MapAreas getAreaId();
	Vector3 getTownCoords();
	float getTownRadius();
	void setTownCoords(Vector3 coords);
	void setTownRadius(float radius);
	const char* randomPedModel();
	const char* getDisplayName();

	tm getLastEncounterTime();
	void setLastEncounterTime(tm time);

	vector<pair<Vector3, float>>* getQuestGiversLocations();
	pair<Vector3, float> getClosestQuestGiverLocation(Vector3 to);
	void addQuestGiverLocation(Vector3 location, float heading);

	vector<MissionTemplateType>* getMissionTemplates();
	void addMissionTemplate(MissionTemplateType templateType);

	vector<Vector3>* getWildernessLocations();
	Vector3 getRandomWildernessLocation();
	void addWildernessLocation(Vector3 location);

	Vector3 getRandomPostOfficeLocation();
	void addPostOfficeLocation(Vector3 location);
	Vector3 getClosestPostOfficeTo(Vector3 location);

	Vector3 randomLocationInTown();
	void addSaloonLocation(Vector3 interiorLocation);
	Vector3 getRandomSaloonLocation();

	void addJailCellLocation(Vector3 location, float heading);
	pair<Vector3, float> getRandomJailCellLocation();

	void addWildernessCampingLocation(Vector3 location);
	Vector3 getRandomCampingLocation(bool allowGenerataion = false);
	vector<Vector3>* listCampingLocations();

	void addRiderRoadLocation(Vector3 location);
	Vector3 getRandomRiderRoadLocation();
	vector<Vector3>* listRiderRoads();

	const char* getRandomLocalGangPedModel();
	void addLocalGangPedModel(const char* model);

	Place* getRandomPlaceByType(PlaceType type);
	vector<Place*> listPlaces();
	Place* getClosestPlaceTo(Vector3 coord);

	vector<Vector3> listTrainTrackLocations();
	void addTrainTrackLocation(Vector3 trainTrackLocation, bool flipDirection);
	pair<Vector3, bool> getRandomTrainTrackLocation();
	pair<Vector3, bool> getClosestTrainTrackTo(Vector3 location, float minDistance = 0);
	Vehicle createTrainHeadingTowardsLocation(TrainConfiguration trainConfig, Vector3 location);
};