#pragma once

#include "AsyncCompanionTask.h"

class DogCompanionInterface
{
protected:
	Ped companion;
	Stopwatch timer;
	Ped trackedPed;

public:
	DogCompanionInterface(Ped companion);
	Ped getPed();
	void stay();
	void follow();
	void fetch(Entity entity, int timeout = 10000);
	WaitOutsideInteriorAsyncTask* waitOutsideInterior();
	AsyncCompanionTask* warn(Entity entity, int timeout);
	AsyncCompanionTask* warnSnake(Entity entity, int timeout);
	void dismiss();
	void getPraised(Entity ped);
	void combat(Ped target);
	void eat();
	void beg(int duration = -1, Entity lookAt = NULL);
	AsyncCompanionTask* track(Ped ped);
	void feedFromInventory(int itemHash, int satchelTextureHash);
	void getPreyMeal(Ped from);
	bool isEntityEatable(Entity entity);
	void playVocalization(const char* vocalization = "BARK");
	bool isVocalizationPlaying();
	bool isPerformingScenario();
	bool isPaused();
	bool isSittingDown();
	void setName(const char* name);
	bool isAgitated();
	bool isInCombat();
	float getHealthRate();
	bool isBegging();
	void hunt(Ped target, float timeout);
	void flee(Ped threat, int duration = 20000, float distance = 50);
	Ped getTrackedPed();
	void clearTrackedPed();
};