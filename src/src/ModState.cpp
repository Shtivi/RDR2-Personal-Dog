#include "Main.h"

ModState::ModState(const char* datafile, MapAreasFactory* mapAreas, bool autosave)
{
	this->dataFile = datafile;
	this->hasChanges = false;
	this->pendingReward = 0;
	this->totalMissions = 0;
	this->autosaveEnabled = autosave;
	this->mapAreas = mapAreas;

	load();
}

void ModState::completeMission(int id, int reward)
{
	setMissionStatus(id, MissionStatus::Completed);
	addReward(reward);
	autosave();
}

void ModState::abortMission(int id)
{
	setMissionStatus(id, MissionStatus::Unavailable);
	autosave();
}

void ModState::acceptMission(int id)
{
	setMissionStatus(id, MissionStatus::Accepted);
	autosave();
}

void ModState::createMission(MissionData* missionData)
{
	missions[missionData->id] = missionData;
	setMissionStatus(missionData->id, MissionStatus::Accepted);
	totalMissions++;
	hasChanges = true;
	autosave();
}

int ModState::getPendingReward()
{
	return pendingReward;
}

void ModState::setPendingReward(int reward)
{
	if (reward == getPendingReward())
	{
		return;
	}

	hasChanges = true;
	pendingReward = reward;
}

int ModState::addReward(int reward)
{
	setPendingReward(getPendingReward() + reward);
	return getPendingReward();
}

void ModState::clearReward()
{
	setPendingReward(0);
	autosave();
}

bool ModState::doesMissionExist(int id)
{
	return missions.find(id) != missions.end();
}

MissionData* ModState::getMission(int id)
{
	if (!doesMissionExist(id))
	{
		return NULL;
	}

	return missions[id];
}

MissionStatus ModState::getMissionStatus(int id)
{
	if (!doesMissionExist(id))
	{
		return MissionStatus::Unavailable;
	}

	return missionStatuses[id];
}

vector<int> ModState::listActiveMissions()
{
	vector<int> activeMissions;
	for (map<int, MissionStatus>::iterator itr = missionStatuses.begin(); itr != missionStatuses.end(); itr++)
	{
		int currId = itr->first;
		MissionStatus currStatus = itr->second;

		if (currStatus == MissionStatus::Accepted)
		{
			activeMissions.push_back(currId);
		}
	}

	return activeMissions;
}

int ModState::getTotalMissions()
{
	return this->totalMissions;
}

int ModState::countActiveMissionsInArea(MapAreas area)
{
	int result = 0;
	vector<int> missions = listActiveMissions();
	for (vector<int>::iterator itr = missions.begin(); itr != missions.end(); itr++)
	{
		int id = *itr;
		if (this->getMission(id)->area == area)
		{
			result++;
		}
	}

	return result;
}

void ModState::setMissionStatus(int id, MissionStatus status)
{
	if (getMissionStatus(id) == status) {
		return;
	}

	missionStatuses[id] = status;
	hasChanges = true;
}

void ModState::save(bool force)
{
	if (!force && !hasChanges)
	{
		return;
	}

	log("saving mod progress");
	std::map<int, MissionData*>::iterator it;
	std::ofstream file;

	file.open(dataFile);;
	if (!file.is_open())
	{
		log("failed to open data file");
		return;
	}

	file << totalMissions << "\n";
	file << pendingReward;

	if (missions.size())
	{
		file << "\n";
	}

	it = missions.begin();
	while (it != missions.end())
	{
		file << serializeMission(it->second, missionStatuses[it->first]);
		
		it++;
		if (it != missions.end())
		{
			file << "\n";
		}
	}

	hasChanges = false;
	file.close();
}

void ModState::load()
{
	std::ifstream file;
	std::string input;
	int count = 0;

	file.open(dataFile);;
	if (file.is_open())
	{
		log("loading mod progress from data file");

		std::getline(file, input);
		this->totalMissions = stoi(input);

		std::getline(file, input);
		this->pendingReward = stoi(input);

		while (!file.eof())
		{
			std::getline(file, input);
			handleMissionInput(input);
			count++;
		}

		file.close();
		string text = string("mod progress loaded: ").append(to_string(count)).append(" missions in total.");
		log(text);
	}
	else
	{
		log("unable to load from data file");
		throw "unable to load from data file";
	}
}

void ModState::handleMissionInput(string missionInputLine)
{
	vector<string> missionProps = split(missionInputLine, ';');

	MapAreas area = static_cast<MapAreas>(stoi(missionProps[2]));

	MissionData* missionData = new MissionData(mapAreas->getMapArea(area));
	missionData->id = stoi(missionProps[0]);
	missionData->templateType = static_cast<MissionTemplateType>(stoi(missionProps[1]));
	missionData->area = area;
	missionData->startPosition = parseVector3(missionProps[3]);
	missionData->areaRadius = stoi(missionProps[4]);
	missionData->identificationDistance = stoi(missionProps[5]);
	missionData->optionalObjectives = parseOptionalObjectives(missionProps[6]);
	missionData->difficultyLevel = static_cast<MissionDifficulty>(stoi(missionProps[7]));

	MissionStatus status = static_cast<MissionStatus>(stoi(missionProps[8]));

	missions[missionData->id] = missionData;
	missionStatuses[missionData->id] = status;
}

const char* ModState::serializeMission(MissionData* missionData, MissionStatus status)
{
	Vector3 startPos = missionData->startPosition;
	stringstream output;
	output << missionData->id << ";"
		<< static_cast<int>(missionData->templateType) << ";"
		<< static_cast<int>(missionData->area) << ";"
		<< startPos.x << "," <<startPos.y << "," << startPos.z << ";"
		<< missionData->areaRadius << ";"
		<< missionData->identificationDistance << ";"
		<< serializeOptionalObjectives(missionData->optionalObjectives)->c_str() << ";"
		<< static_cast<int>(missionData->difficultyLevel) << ";"
		<< static_cast<int>(status);
	return output.str().c_str();
}

vector<OptionalObjectives>* ModState::parseOptionalObjectives(string input)
{
	if (input.empty())
	{
		return OptionalObjective::random(3);
	}
	else
	{
		vector<OptionalObjectives>* results = new vector<OptionalObjectives>();
		vector<string> components = split(input, ',');
		for (string component : components)
		{
			results->push_back(static_cast<OptionalObjectives>(stoi(component)));
		}

		return results;
	}
}

string* ModState::serializeOptionalObjectives(vector<OptionalObjectives>* optionalObjectives)
{
	string* result = new string();
	for (vector<OptionalObjectives>::iterator itr = optionalObjectives->begin(); itr != optionalObjectives->end(); itr++)
	{
		OptionalObjectives curr = *itr;
		result->append(to_string((int)curr));
		
		if (itr + 1 != optionalObjectives->end())
		{
			result->append(",");
		}
	}

	return result;
}


void ModState::autosave()
{
	if (autosaveEnabled)
	{
		log("Autosaving");
		save();
	}
}