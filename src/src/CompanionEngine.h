#pragma once

#include "DogCompanionInterface.h"
#include "RootState.h"
#include "CompanionCommands.h"

class CompanionEngine
{
private:
	RootState* state;

public:
	CompanionEngine();
	void start();
	void update();
	RootState* getState();

private:
	Ped spawnDog();
	void updatePrompts();
	void scanCompanionSurrounding();
	void updateGUI();
	void updateCompanionStats();
	void onAnimalInteraction(int eventIndex);
	void onBulletImpact(int eventIndex);
	void onPromptTriggered(int eventIndex);
	void onWhistle(int eventIndex);
	void onCompnaionDied();
	void onPredatorDetected(Ped predator);
	void bondWithDog();
	void accompanyDog(Ped dog);
	void dismissCompanion();
	void clearCompanion();
	void renameCompanion(const char* name);
	void tutorial(const char* tutorialName);
};