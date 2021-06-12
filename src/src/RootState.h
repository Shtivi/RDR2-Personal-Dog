#pragma once

#include "CompanionState.h"

class RootState
{
public:
	Ped candidateDog = NULL;
	Ped companionDog = NULL;
	Prompts* accompanyPrompt = NULL;
	Prompts* stayPrompt = NULL;
	Prompts* feedPrompt = NULL;
	Prompts* praisePrompt = NULL;
	Prompts* dismissPrompt = NULL;
	Prompts* followPrompt = NULL;
	Prompts* renamePrompt = NULL;
	Prompts* attackPrompt = NULL;
	Prompts* retrieveDogPrompt = NULL;
	ComapnionCoresUI* coresUI = NULL;
	DogCompanionInterface* companionApi = NULL;
	AsyncCompanionTask* currentTask;
	Blip companionBlip;
	float previousCompanionHealth;
	Stopwatch calmTimer;
	Stopwatch statsTimer;
	bool wasInsideInterior;
	bool isWithinWhistlingRange;
	bool didPlayerHadControlLastFrame;
};