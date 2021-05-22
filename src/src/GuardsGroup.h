#pragma once

class GuardsGroup
{
private:
	vector<GenericGuardingBehavior*> guards;
	map<Ped, TensionMode> guardsModeSnapshot;
	Vector3 defensePosition;
	float radius;
	bool shouldTolerate;
	set<Ped> bodiesFound;

public:
	GuardsGroup(Vector3 defensePosition, float radius, bool shouldTolerate = true);

	void add(Ped ped, IdlingModifier idlingModifier = IdlingModifier::Scout);
	void add(Ped ped, IdlingModifier idlingModifier, RoutineParams routineParams);
	vector<Ped>* peds();
	void start(bool withBlips = false);
	void stop();
	void update();
	void alert();
	void search();
	void combat();
	void addBlips();
	void removeBlips();

private:
	void clearDeadGuards();
	TensionMode detectSharedTensionMode();
	bool shouldModeBeShared(TensionMode mode);
};