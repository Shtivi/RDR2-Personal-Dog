#pragma once

#include "PaymentExecutor.h"
#include "QuestOfferExecutor.h"
#include "AssassinationMissionExecutor.h"

class MissionsEngine
{
private:
	MapAreasFactory* areasFactory;
	ModState* progress;
	MissionDataFactory* missionsFactory;
	ContractsMenu* menu;
	map<int, AssassinationMissionExecutor*> activeMissionExecutors;
	map<int, tm> failedMissions;
	vector<QuestOfferExecutor*> questOfferExecutors;
	PaymentExecutor* paymentExecutor;
	Stopwatch stopwatch;

public:
	MissionsEngine(MapAreasFactory* areasFactory, MissionDataFactory* missionsFactory, ModState* progress, PaymentExecutor* paymentExecutor, ContractsMenu* menu);
	void update();
	void abortMission(int missionId);

private:
	int countActiveMissions();
	void loadMissionExecutors();
	void updateActiveExecutors();
	void updateFailedExecutors();
	void updateAbortedExecutors();
	void triggerQuestOffers();
	void updateQuestOffers();

	AssassinationMissionExecutor* createExecutor(MissionData* mission, MissionStatus status);
};