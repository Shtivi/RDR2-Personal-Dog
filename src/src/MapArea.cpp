#include "Main.h"

MapArea::MapArea(MapAreas areaId, vector<const char*> pedModels, const char* displayName, vector<Place*> places)
{
	id = areaId;
	lastEncounterTime = tm();
	this->pedModels = pedModels;
	this->displayName = displayName;
	this->places = places;
}

MapAreas MapArea::getAreaId()
{
	return id;
}

Vector3 MapArea::getTownCoords()
{
	return townCoords;
}

float MapArea::getTownRadius()
{
	return townRadius;
}

void MapArea::setTownCoords(Vector3 coords)
{
	townCoords = coords;
}

void MapArea::setTownRadius(float radius)
{
	townRadius = radius;
}

vector<pair<Vector3, float>>* MapArea::getQuestGiversLocations()
{
	return &(this->questGiversLocations);
}

vector<MissionTemplateType>* MapArea::getMissionTemplates()
{
	return &(this->missionTemplates);
}

void MapArea::addQuestGiverLocation(Vector3 location, float heading)
{
	questGiversLocations.push_back(make_pair(location, heading));
}

void MapArea::addMissionTemplate(MissionTemplateType templateType)
{
	missionTemplates.push_back(templateType);
}

tm MapArea::getLastEncounterTime()
{
	return lastEncounterTime;
}

void MapArea::setLastEncounterTime(tm time)
{
	lastEncounterTime = time;
}

void MapArea::addWildernessLocation(Vector3 location)
{
	wildernessLocations.push_back(location);
	addRiderRoadLocation(location);
}

vector<Vector3>* MapArea::getWildernessLocations()
{
	return &(this->wildernessLocations);
}

Vector3 MapArea::getRandomWildernessLocation()
{
	int index = rndInt(0, wildernessLocations.size());
	return wildernessLocations[index];
}

pair<Vector3, float> MapArea::getClosestQuestGiverLocation(Vector3 to)
{
	pair<Vector3, float> best = questGiversLocations[0];
	float bestDistance = distance(best.first, to);

	for (vector<pair<Vector3, float>>::iterator itr = questGiversLocations.begin(); itr != questGiversLocations.end(); itr++)
	{
		float currDistance = distance((*itr).first, to);
		if (currDistance < bestDistance)
		{
			best = *itr;
			bestDistance = currDistance;
		}
	}

	return best;
}

const char* MapArea::randomPedModel()
{
	return pedModels[rndInt(0, pedModels.size())];
}

Vector3 MapArea::getRandomPostOfficeLocation()
{
	return postOfficeLocations.at(rndInt(0, postOfficeLocations.size()));
}

void MapArea::addPostOfficeLocation(Vector3 location)
{
	postOfficeLocations.push_back(location);
}

Vector3 MapArea::getClosestPostOfficeTo(Vector3 location)
{
	if (!postOfficeLocations.size())
	{
		return toVector3(0, 0, 0);
	}

	Vector3 best = postOfficeLocations.at(0);
	float bestDistance = distance(best, location);
	for (vector<Vector3>::iterator itr = postOfficeLocations.begin(); itr != postOfficeLocations.end(); itr++)
	{
		Vector3 current = *itr;
		float currentDistance = distance(current, location);
		if (currentDistance < bestDistance)
		{
			best = current;
			bestDistance = currentDistance;
		}
	}

	return best;
}

Vector3 MapArea::randomLocationInTown()
{
	//Vector3 townLocation = questGiversLocations.at(rndInt(0, questGiversLocations.size())).first;
	//int radius = rndInt(0, 20);
	//return around(townLocation, radius, true);

	Vector3 townLocation = questGiversLocations.at(rndInt(0, questGiversLocations.size())).first;
	return townLocation;
}

void MapArea::addSaloonLocation(Vector3 location)
{
	saloonLocations.push_back(location);
}

Vector3 MapArea::getRandomSaloonLocation()
{
	return saloonLocations.at(rndInt(0, saloonLocations.size()));
}

void MapArea::addJailCellLocation(Vector3 location, float heading)
{
	jailCellLocations.push_back(make_pair(location, heading));
}

pair<Vector3, float> MapArea::getRandomJailCellLocation()
{
	return jailCellLocations.at(rndInt(0, jailCellLocations.size()));
}

const char* MapArea::getDisplayName()
{
	return displayName;
}

void MapArea::addWildernessCampingLocation(Vector3 location)
{
	wildrenessCampingLocations.push_back(location);
}

Vector3 MapArea::getRandomCampingLocation(bool allowGenerataion)
{
	bool willGenerate = GAMEPLAY::GET_RANDOM_FLOAT_IN_RANGE(0, 1) > 0.5;
	if (allowGenerataion)
	{
		willGenerate = false;
	}

	if (willGenerate)
	{
		Vector3 source = getRandomWildernessLocation();
		int radius = rndInt(15, 35);
		Vector3 generatedLocation = around(source, radius);
		Vector3* safeLocation = getSafeCoordForPed(generatedLocation);
		if (safeLocation)
		{
			return *safeLocation;
		}

		return getRandomCampingLocation(false);
	}
	else
	{
		return wildrenessCampingLocations[rndInt(0, wildrenessCampingLocations.size())];
	}
}

vector<Vector3>* MapArea::listCampingLocations()
{
	return &wildrenessCampingLocations;
}

const char* MapArea::getRandomLocalGangPedModel()
{
	return localGangModels[rndInt(0, localGangModels.size())];
}

void MapArea::addLocalGangPedModel(const char* model)
{
	localGangModels.push_back(model);
}

void MapArea::addRiderRoadLocation(Vector3 location)
{
	riderRoadLocations.push_back(location);
}

Vector3 MapArea::getRandomRiderRoadLocation()
{
	int index = rndInt(0, riderRoadLocations.size());
	return wildernessLocations[index];
}

vector<Vector3>* MapArea::listRiderRoads()
{
	return &riderRoadLocations;
}

Place* MapArea::getRandomPlaceByType(PlaceType type)
{
	vector<Place*> filtered;
	for (vector<Place*>::iterator itr = places.begin(); itr != places.end(); itr++)
	{
		Place* current = *itr;
		if (current->getPlaceType() == type)
		{
			filtered.push_back(current);
		}
	}

	if (filtered.empty())
	{
		return NULL;
	}

	return filtered.at(rndInt(0, filtered.size()));
}

vector<Place*> MapArea::listPlaces()
{
	return this->places;
}

Place* MapArea::getClosestPlaceTo(Vector3 coord)
{
	if (places.empty())
	{
		return NULL;
	}

	Place* best = places.at(0);
	float bestDistance = distance(coord, best->getCoords());

	for (vector<Place*>::iterator itr = places.begin(); itr != places.end(); itr++)
	{
		Place* current = *itr;
		float currentDistance = distance(current->getCoords(), coord);
		if (currentDistance < bestDistance)
		{
			best = current;
			bestDistance = currentDistance;
		}
	}

	return best;
}

vector<Vector3> MapArea::listTrainTrackLocations()
{
	vector<Vector3> result;
	for (vector<pair<Vector3, bool>>::iterator itr = trainTrackLocations.begin(); itr != trainTrackLocations.end(); itr++)
	{
		result.push_back(itr->first);
	}

	return result;
}

void MapArea::addTrainTrackLocation(Vector3 trainTrackLocation, bool flip)
{
	trainTrackLocations.push_back(pair<Vector3, bool>(trainTrackLocation, flip));
}

pair<Vector3, bool> MapArea::getRandomTrainTrackLocation()
{
	if (!trainTrackLocations.size())
	{
		return pair<Vector3, bool>(toVector3(0, 0, 0), false);
	}

	return trainTrackLocations.at(rndInt(0, trainTrackLocations.size()));
}

pair<Vector3, bool> MapArea::getClosestTrainTrackTo(Vector3 location, float minDistance)
{
	if (!trainTrackLocations.size())
	{
		return pair<Vector3, bool>(toVector3(0, 0, 0), false);
	}

	Vector3 best = toVector3(0, 0, 0);
	bool bestFlip = false;
	float bestDistance = 99999;

	for (vector<pair<Vector3, bool>>::iterator itr = trainTrackLocations.begin(); itr != trainTrackLocations.end(); itr++)
	{
		pair<Vector3, bool> curr = *itr;
		float currDistance = distance(curr.first, location);
		if (currDistance < bestDistance && currDistance >= minDistance)
		{
			best = curr.first;
			bestFlip = curr.second;
			bestDistance = currDistance;
		}
	}

	return pair<Vector3, bool>(best, bestFlip);
}

Vehicle MapArea::createTrainHeadingTowardsLocation(TrainConfiguration trainConfig, Vector3 location)
{
	pair<Vector3, bool> locationTracks = getClosestTrainTrackTo(location);
	pair<Vector3, bool> spawn = getClosestTrainTrackTo(locationTracks.first, 10);	
	bool direction = locationTracks.first.x < spawn.first.x;
	if (spawn.second) 
	{
		direction = !direction;
	}

	Vehicle train = createTrain(TrainConfiguration::Passangers, spawn.first, direction);
	return train;
}