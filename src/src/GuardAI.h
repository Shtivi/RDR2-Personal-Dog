#pragma once

#include "DangerousEvent.h"

class GuardAI : public PedBehavior
{
private:
	set<Ped> ignoreableBodies;
	float visionRange;
	float hearingRange;

public:
	GuardAI(Ped ped, int visionRange, int hearingRange, bool persistBlip = false);

	bool useNearestScenarioPoint();
	void useScenarioPoint(int scenarioPoint, bool immediately = false);
	bool isSleeping();
	bool canShoot();
	bool hasScenarioAvailable(float distance);
	bool canDetect(Ped enemy);
	void search(Vector3 around, float radius);
	void patrol(Vector3 area, float radius);
	void warn(Ped suspect);
	void threaten(Ped enemy, bool speak = true);
	bool isThreatenedByPlayer();
	void cower(Ped from);
	DangerousEvent detectEventsAround();
	void addIgnorableBody(Ped deadBody);
	void addIgnorableBodies(vector<Ped> deadBodies);
	bool shouldIgnoreBody(Ped deadBody);
	vector<Ped> detectBodiesNearby();
};