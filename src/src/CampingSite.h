#pragma once

#include "GuardAI.h"

enum class CampAlertnessStatus : int
{
	Idle = 0,
	Suspect,
	Warn,
	Alerted,
	Search,
	Combat,
	Flee
};

enum class CampVariant : int
{
	Campfire,
	Tent
};

class CampingSite
{
private:
	Vector3 location;
	int defensiveRadius;
	int tolerationTime;
	CampVariant variant;
	CampAlertnessStatus alertnessStatus;
	
	Stopwatch stopwatch;
	vector<Entity> props;
	vector<GuardAI*> campers;
	vector<GuardAI*> guards;
	vector<GuardAI*> allPeds;
	vector<int> idleScenarios;
	vector<int> guardScenarios;
	
	GuardAI* alertedGuard;

public:
	CampingSite(Vector3 location, int defensiveRadius = 25, int tolerationTime = 8, CampVariant variant = CampVariant::Campfire);
	
	void play();
	void update();
	void addCamper(Ped ped);
	void addGuard(Ped ped, bool persistBlip = false);
	void showBlips();
	void removeBlips();
	void cleanup();

protected:
	void createPropset();
	Object addProp(Object prop);
	const char* generateSeatedScenario();
	void updateEvents();
	void releaseUnnecessaryEntities();
	bool anyGuardsLeft();
	GuardAI* findAlertableGuard();
	bool isGroupThreatened();

	void enterIdleMode(bool jumpIntoScenarios = false);
	void enterSuspiciousMode();
	void enterWarningMode();
	void enterAlertedMode();
	void enterSearchMode();
	void enterCombatMode();
	void enterFleeMode();

private:
	void createPropsetCampfire();
	void createPropsetTent();
};