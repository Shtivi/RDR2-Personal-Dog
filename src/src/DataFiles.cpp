#include "Main.h"

const char* DataFiles::DataFilesPath = "DogCompanion";
DictDataFile* DataFiles::Dog = NULL;
DictDataFile* DataFiles::DogMeta = NULL;
DictDataFile* DataFiles::Lang = NULL;
DictDataFile* DataFiles::TutorialFlags = NULL;

const char* DataFiles::getDataFilePath(const char* filename)
{
	return (new string(string(DataFilesPath).append("/").append(filename).c_str()))->c_str();
}

bool DataFiles::load()
{
	Dog = new DictDataFile(getDataFilePath("Dog.dat"));
	if (!Dog->load())
	{
		return false;
	}

	Lang = new DictDataFile(getDataFilePath("Lang.dat"));
	if (!Lang->load())
	{
		return false;
	}

	DogMeta = new DictDataFile(getDataFilePath("DogMeta.dat"));
	if (!DogMeta->load())
	{
		return false;
	}

	TutorialFlags = new DictDataFile(getDataFilePath("TutorialFlags.dat"));
	if (!TutorialFlags->load())
	{
		log("TutorialFlags didn't load properly, continuing anyway.");
	}

	return true;
}