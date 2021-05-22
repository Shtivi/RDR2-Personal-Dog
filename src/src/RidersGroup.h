#pragma once

class RidersGroup
{
private:
	RiderAI* leader;
	vector<RiderAI*> members;
	vector<PassangerAI*> passangers;
	int warningTimeTolerance;
	int safeDistance;
	AlertnessStatus status;
	Stopwatch stopwatch;
	bool wasAlerted = true;
	bool toleratePlayer = true;

public:
	RidersGroup(Ped leader, int safeDistance, int warningTimeTolerance, bool persistLeaderBlip = false);

	int getSafeDistance();
	void setSafeDistance(int distance);
	int getWarningTimeTolerance();
	void setWarningTimeTolerance(int time);
	AlertnessStatus getAlertnessStatus();
	void setAlertnessStatus(AlertnessStatus alertnessStatus);
	
	void addRider(Ped rider);
	void addPassanger(Ped passanger);
	void update();
	void showEnemyBlips();
	void removeEnemyBlips();

protected:
	int getGroupId();
	void setLeader(RiderAI* leader, int groupId = 0);
	void addPed(Ped ped);
	void addPed(RiderAI* ped);
	RiderAI* findClosestMemberTo(Vector3 location);
	void releaseUnnecessaryEntities();
	bool isGroupThreatenedByPlayer();
	bool contains(Ped ped);
	float distanceTo(Vector3 location);
	void updatePassangers();
	bool areGuardsDead();

	void enterIdleMode();
	void enterWarningMode(Ped suspect);
	void enterThreateningMod(Ped suspect);
	void enterAlertedMode();
	void enterCombatMode(Ped ped);
	void enterFleeMode(Ped enemy);
};