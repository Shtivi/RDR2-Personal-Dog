#pragma once

#include "BaseDataFile.h"
#include "DictDataFile.h"

class DataFiles
{
public:
	static const char* DataFilesPath;
	static const char* getDataFilePath(const char* filename);
	static bool load();
	static DictDataFile* Dog;
	static DictDataFile* Lang;
	static DictDataFile* DogMeta;
	static DictDataFile* TutorialFlags;
};
