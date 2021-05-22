#include "Main.h";

using namespace std;

ModProgress::ModProgress(const char* dataFile)
{
	hasChanges = false;
	this->dataFile = dataFile;
	readFromDataFile();
}

void ModProgress::completeMission(int missionId)
{
	//setMissionProgress(missionId, MissionStatus::Done);
}

void ModProgress::startMission(int missionId)
{
	//setMissionProgress(missionId, MissionStatus::InProgress);
}

void ModProgress::save()
{
	std::map<int, MissionStatus>::iterator it;
	std::ofstream file;


	if (!hasChanges)
	{
		return;
	}

	log("saving mod progress");

	file.open(dataFile);;
	if (file.is_open())
	{
		it = cache.begin();

		while (it != cache.end())
		{
			std::stringstream output;
			; output << it->first << " " << static_cast<int>(it->second);
			file << output.str();

			it++;
			if (it != cache.end())
			{
				file << "\n";
			}
		}

		hasChanges = false;
		file.close();
	}
}

MissionStatus ModProgress::getMissionProgress(int missionId)
{
	std::map<int, MissionStatus>::iterator it = cache.find(missionId);

	if (it == cache.end())
	{
		setMissionProgress(missionId, MissionStatus::Accepted);
		save();
		return MissionStatus::Accepted;
	}

	return it->second;;
}

void ModProgress::setMissionProgress(int missionId, MissionStatus status)
{
	std::map<int, MissionStatus>::iterator it = cache.find(missionId);

	if (it == cache.end())
	{
		cache[missionId] = status;
		hasChanges = true;
	}
	else
	{
		if (it->second != status)
		{
			hasChanges = true;
			it->second = status;
		}
	}

}

void ModProgress::readFromDataFile()
{
	std::ifstream file;
	std::string input;
	int count = 0;

	file.open(dataFile);;
	if (file.is_open())
	{
		log("loading mod progress from data file");

		while (!file.eof())
		{
			std::getline(file, input);
			parseToCache(input);
			count++;
		}

		file.close();
		string text = string("mod progress loaded: ").append(to_string(count)).append(" missions");
		log(text);
	}
	else
	{
		log("unable to load from data file");
		throw "unable to load from data file";
	}
}

void ModProgress::parseToCache(std::string inputLine)
{
	std::string idStr, statusStr;
	int id, status;

	int splitPos = inputLine.find_first_of(' ');

	if (splitPos == std::string::npos)
	{
		throw "Invalid input line";
	}

	idStr = inputLine.substr(0, splitPos);
	statusStr = inputLine.substr(splitPos + 1);

	id = std::stoi(idStr);
	status = std::stoi(statusStr);

	cache[id] = static_cast<MissionStatus>(status);
}