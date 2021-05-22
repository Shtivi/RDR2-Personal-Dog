#pragma once

class OptionalObjective
{
public:
	OptionalObjectives id;
	int reward;
	const char* displayName;
	const char* textureDictionary;
	const char* textureName;
	const char* description;

	static OptionalObjective* fromID(OptionalObjectives id);
	static vector<OptionalObjective*>* list();
	static vector<OptionalObjectives>* listIds();
	static vector<OptionalObjectives>* random(int count = 3);
	OptionalObjective(OptionalObjectives id, int reward, const char* displayName, const char* textureDict, const char* textureName, const char* description);
};