#pragma once

#include "DogCompanionInterface.h"
#include "RootState.h"

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
	void updatePrompts();
	void scanCompanionSurrounding();
	void updateGUI();
	void onAnimalInteraction(int eventIndex);
	void onBulletImpact(int eventIndex);
	void onPromptTriggered(int eventIndex);
	void onWhistle(int eventIndex);
	void onCompnaionDied();
	void onPredatorDetected(Ped predator);
	void bondWithDog();
	void accompanyDog(Ped dog);
	void triggerFollow();
	void dismissCompanion();
	void clearCompanion();
	void renameCompanion(const char* name);
	void tutorial(const char* tutorialName);
};