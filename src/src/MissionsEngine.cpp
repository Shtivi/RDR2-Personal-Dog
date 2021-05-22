#include "Main.h"

const int RANDOM_ENCOUNTERS_TIME_GAP = 28800; // 8 Hours
const int FAILED_MISSIONS_COOLDOWN_HOURS = 10; 

MissionsEngine::MissionsEngine(MapAreasFactory* areasFactory, MissionDataFactory* missionsFactory, ModState* progress, PaymentExecutor* paymentExecutor, ContractsMenu* menu)
{
	this->areasFactory = areasFactory;
	this->missionsFactory = missionsFactory;
	this->progress = progress;
	this->paymentExecutor = paymentExecutor;
	this->menu = menu;

	loadMissionExecutors();
}

void MissionsEngine::update()
{
	updateActiveExecutors();
	updateQuestOffers();
	triggerQuestOffers();
	updateFailedExecutors();
	updateAbortedExecutors();
	paymentExecutor->update();

	if (stopwatch.getElapsedSeconds() > 5)
	{
		stopwatch.stop();
	}
	else if (stopwatch.isStarted())
	{
		if (CONTROLS::IS_CONTROL_JUST_PRESSED(0, GAMEPLAY::GET_HASH_KEY("INPUT_FEED_INTERACT")))
		{
			menu->openMission(progress->getTotalMissions());
		}
	}
}

void MissionsEngine::abortMission(int missionId)
{
	progress->abortMission(missionId);
	if (activeMissionExecutors.find(missionId) != activeMissionExecutors.end())
	{
		activeMissionExecutors[missionId]->setStatus(MissionStatus::Unavailable);
	}

	stringstream logMsg;
	logMsg << "Mission has been aborted: " << missionId;
	log(logMsg.str().c_str());
}

int MissionsEngine::countActiveMissions()
{
	return activeMissionExecutors.size() + failedMissions.size();
}

void MissionsEngine::loadMissionExecutors()
{
	vector<int> activeMissions = this->progress->listActiveMissions();
	for (vector<int>::iterator itr = activeMissions.begin(); itr != activeMissions.end(); itr++)
	{
		int currId = *itr;
		MissionStatus initialStatus = progress->getMissionStatus(currId);
		MissionData* mission = progress->getMission(currId);
		activeMissionExecutors[currId] = createExecutor(mission, initialStatus);
	}
	log(string("initialized mission executors for: ").append(to_string(activeMissions.size()).append(" active missions").c_str()));
}

void MissionsEngine::updateActiveExecutors()
{
	map<int, AssassinationMissionExecutor*>::iterator itr = activeMissionExecutors.begin();
	while (itr != activeMissionExecutors.end())
	{
		int id = (*itr).first;
		AssassinationMissionExecutor* executor = (*itr).second;

		if (GAMEPLAY::GET_MISSION_FLAG())
		{
			if (executor->getStatus() != MissionStatus::Blocked)
			{
				executor->block();
			}
		} 
		else
		{
			if (executor->getStatus() == MissionStatus::Blocked)
			{
				float playerDistance = distance(executor->getMissionData()->startPosition, playerPos());
				if (playerDistance > executor->getMissionData()->areaRadius * 2)
				{
					executor->resume();
				}
			}
			else 
			{
				executor->update();
			}
		}

		if (executor->getStatus() == MissionStatus::Completed)
		{
			progress->completeMission(id, executor->getTotalReward());
			itr = activeMissionExecutors.erase(itr);
			areasFactory->getMapArea(executor->getMissionData()->area)->setLastEncounterTime(getGameTime());

			stringstream logMsg;
			logMsg << "Contract Completed: " << id;
			log(logMsg.str().c_str());
		}
		else if (executor->getStatus() == MissionStatus::Failed)
		{
			failedMissions[id] = getGameTime();
			itr = activeMissionExecutors.erase(itr);
			areasFactory->getMapArea(executor->getMissionData()->area)->setLastEncounterTime(getGameTime());
			showHelpText("Failed missions will be available for you again within a few hours.");
		}
		else if (executor->getStatus() == MissionStatus::Unavailable)
		{
			executor->setStatus(MissionStatus::Unavailable);
			itr = activeMissionExecutors.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void MissionsEngine::updateFailedExecutors()
{
	map<int, tm>::iterator itr = failedMissions.begin();
	tm now = getGameTime();
	while (itr != failedMissions.end())
	{
		int id = (*itr).first;
		tm failureTime = (*itr).second;
		float elapsedHours = Stopwatch::getElapsedSeconds(now, failureTime) / 3600;
		if (elapsedHours > FAILED_MISSIONS_COOLDOWN_HOURS)
		{
			activeMissionExecutors[id] = createExecutor(progress->getMission(id), progress->getMissionStatus(id));
			itr = failedMissions.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}

void MissionsEngine::updateAbortedExecutors()
{
	for (pair<int, AssassinationMissionExecutor*> executor : activeMissionExecutors)
	{
		if (progress->getMissionStatus(executor.first) == MissionStatus::Unavailable && executor.second->getStatus() != MissionStatus::Unavailable)
		{
			executor.second->setStatus(MissionStatus::Unavailable);
			stringstream logMsg;
			logMsg << "Mission has been aborted: " << executor.first;
			log(logMsg.str().c_str());
		}
	}
}

void MissionsEngine::triggerQuestOffers()
{
	MapArea* playerArea = areasFactory->getMapArea(getPlayerMapArea());
	if (!playerArea)
	{
		return;
	}

	if (PED::IS_PED_IN_MELEE_COMBAT(player) || 
		getPedEquipedWeapon(player) != WeaponHash::Unarmed || 
		PED::IS_PED_SHOOTING(player) || 
		(int)PURSUIT::_0x259CE340A8738814(PLAYER::PLAYER_ID()) != 0 || 
		PLAYER::GET_PLAYER_WANTED_LEVEL(PLAYER::PLAYER_ID()) > 0 ||
		GAMEPLAY::GET_MISSION_FLAG())
	{
		return;
	}

	double timeSinceLastEncounter = Stopwatch::getElapsedSeconds(playerArea->getLastEncounterTime(), getGameTime());
	pair<Vector3, float>closestQuestGiver = playerArea->getClosestQuestGiverLocation(playerPos());
	if (timeSinceLastEncounter > RANDOM_ENCOUNTERS_TIME_GAP && distance(closestQuestGiver.first, player) <= 30 && progress->countActiveMissionsInArea(playerArea->getAreaId()) == 0)
	{
		Ped questGiverPed = getClosestPed(closestQuestGiver.first, 20);
		if (questGiverPed && 
			ENTITY::DOES_ENTITY_EXIST(questGiverPed) && 
			ENTITY::IS_ENTITY_A_PED(questGiverPed) && 
			PED::IS_PED_ON_FOOT(questGiverPed) && 
			PED::IS_PED_HUMAN(questGiverPed) && 
			!ENTITY::IS_ENTITY_A_MISSION_ENTITY(questGiverPed)
		)
		{
			log(string("Initiating a quest offer in ")
				.append(playerArea->getDisplayName())
				.append(". Quest giver coordinates: ")
				.append(to_string(closestQuestGiver.first)).c_str());
			QuestOfferExecutor* questOffer = new QuestOfferExecutor(playerArea->getAreaId(), questGiverPed);
			questOfferExecutors.push_back(questOffer);
			playerArea->setLastEncounterTime(getGameTime());
			WAIT(1000);
			questOffer->start();
		}
	}
}

void MissionsEngine::updateQuestOffers()
{
	vector<QuestOfferExecutor*>::iterator itr = questOfferExecutors.begin();
	while (itr != questOfferExecutors.end())
	{
		QuestOfferExecutor* curr = *itr;
		if (curr->isRunning())
		{
			curr->update();
			itr++;
		}
		else
		{
			itr = questOfferExecutors.erase(itr);
			if (curr->getOutcome() == QuestOfferOutcome::Accepted)
			{
				int nextId = progress->getTotalMissions() + 1;
				MapArea* area = areasFactory->getMapArea(curr->getArea());
				MissionData* newMission = missionsFactory->generateMission(nextId, area);
				log("mission created successfuly");

				progress->createMission(newMission);
				activeMissionExecutors[nextId] = createExecutor(newMission, MissionStatus::Accepted);

				showAdvancedNotification("Contract Accepted", "Press ~INPUT_FEED_INTERACT~ for more information", "INVENTORY_ITEMS", "generic_note");
				stopwatch.start();
				log("Contract initialization completed.");
			}
			else
			{
				bool shouldDisplayHelpText = rndInt(1, 4) == 1;
				if (curr->getOutcome() == QuestOfferOutcome::Declined && shouldDisplayHelpText)
				{
					showHelpText("You've declined a contract offer. Other offers in this town will be available within a few hours.");
				}
			}
		}
	}
}

AssassinationMissionExecutor* MissionsEngine::createExecutor(MissionData* mission, MissionStatus status)
{
	switch (mission->templateType)
	{
		case MissionTemplateType::TransportedPrisoner:
		{
			return new TransportedPrisonerExecutor(mission, status, areasFactory->getMapArea(mission->area));
		}
		case MissionTemplateType::TrainStation:
		{
			return new TrainStationExecutor(mission, status);
		}
		case MissionTemplateType::BusinessManConvoy: 
		{
			return new BusinessManConvoyExecutor(mission, status);
		}
		case MissionTemplateType::Saloon:
		{
			return new SaloonExecutor(mission, status);
		}
		case MissionTemplateType::JailPrisoner:
		{
			return new JailPrisonerExecutor(mission, status);
		}
		case MissionTemplateType::Lawman:
		{
			return new LawmanExecutor(mission, status);
		}
		case MissionTemplateType::BanditsCamp:
		{
			return new BanditsCampExecutor(mission, status, areasFactory->getMapArea(mission->area));
		}
		case MissionTemplateType::Riders:
		{
			return new RidersExecutor(mission, status, areasFactory->getMapArea(mission->area));
		}
		case MissionTemplateType::Hunter:
		{
			return new HunterExecutor(mission, status);
		}
		case MissionTemplateType::AmbientPed:
		{
			AmbientPedExecutorArgs params;
			params.companions = 1;
			params.lookupRadius = mission->identificationDistance * 1.5;

			return new AmbientPedExecutor(mission, status, areasFactory->getMapArea(mission->area), params);
		}
		case MissionTemplateType::AnnesburgMineManager:
		{
			return new AnnesburgMineManagerExecutor(mission, status);
		}
		case MissionTemplateType::AnnesburgMineGuard:
		{
			AmbientPedExecutorArgs params;
			params.companions = 0;
			params.lookupRadius = mission->identificationDistance * 1.5;
			params.pedModel = "a_m_m_jamesonguard_01";

			return new AmbientPedExecutor(mission, status, areasFactory->getMapArea(mission->area), params);
		}
		case MissionTemplateType::BountyHuntersRiders:
		{
			return new BountyHuntersRidersExecutor(mission, status, areasFactory->getMapArea(mission->area));
		}
		case MissionTemplateType::BanditsShack:
		{
			MapArea* area = areasFactory->getMapArea(mission->area);
			return new GenericShackExecutor(mission, status, area->getClosestPlaceTo(mission->startPosition), area);
		}
		case MissionTemplateType::BanditHideout:
		{
			MapArea* area = areasFactory->getMapArea(mission->area);
			AmbientPedExecutorArgs params;
			params.lookupRadius = mission->identificationDistance * 1.5;

			return new AmbientPedExecutor(mission, status, area, params);
		}
		case MissionTemplateType::AmbientRancher:
		{
			MapArea* area = areasFactory->getMapArea(mission->area);
			Place* ranch = area->getClosestPlaceTo(mission->startPosition);
			AmbientPedExecutorArgs params;
			params.lookupRadius = mission->identificationDistance * 1.5;
			params.pedModel = ranch->getTypicalPedModel() ? ranch->getTypicalPedModel() : "a_m_m_rancher_01";
			params.lookupRadius = ranch->getRadius();

			return new AmbientPedExecutor(mission, status, area, params);
		}
		case MissionTemplateType::ArmyConvoy:
		{
			ConvoyExecutorArgs params;
			params.wagonModel = VehicleHash::ArmySupplyWagon;
			params.cargoHash = VehicleCargoHash::ArmyCargo;
			params.trackdownMessage = "Locate the army convoy and assassinate the soldier.";
			params.enemiesModel = mission->difficultyLevel == MissionDifficulty::Normal ? "s_m_m_army_01" : "s_m_y_army_01";

			return new GenericConvoyExecutor(mission, status, params);
		}
		case MissionTemplateType::MoonshinerConvoy:
		{
			ConvoyExecutorArgs params;
			params.wagonModel = VehicleHash::ChuckWagon000X;
			params.cargoHash = VehicleCargoHash::TonsOfMoonshine;
			params.trackdownMessage = "Locate and kill the moonshiner.";
			params.enemiesModel = "a_m_m_moonshiners_01";

			return new GenericConvoyExecutor(mission, status, params);
		}
		case MissionTemplateType::OilTraderConvoy:
		{
			ConvoyExecutorArgs params;
			params.wagonModel = VehicleHash::OilWagon01X;
			params.trackdownMessage = "Locate and kill the moonshiner.";
			params.enemiesModel = "g_m_m_unicornwallgoons_01";

			return new GenericConvoyExecutor(mission, status, params);
		}
		case MissionTemplateType::Grey:
		{
			MapArea* area = areasFactory->getMapArea(mission->area);
			Place* caligaHall = area->getRandomPlaceByType(PlaceType::CaligaHall);
			AmbientPedExecutorArgs params;
			params.companions = 0;
			params.lookupRadius = caligaHall->getRadius();
			params.pedModel = caligaHall->getTypicalPedModel();

			return new AmbientPedExecutor(mission, status, area, params);
		}
		case MissionTemplateType::Braithwaite:
		{
			MapArea* area = areasFactory->getMapArea(mission->area);
			Place* braithwaite = area->getRandomPlaceByType(PlaceType::BraithwaiteManor);
			AmbientPedExecutorArgs params;
			params.companions = 0;
			params.lookupRadius = braithwaite->getRadius();
			params.pedModel = braithwaite->getTypicalPedModel();
			return new AmbientPedExecutor(mission, status, area, params);
		}
		case MissionTemplateType::TrainTraveler:
		{
			MapArea* area = areasFactory->getMapArea(mission->area);
			return new TrainTravelerExecutor(mission, status, area);
		}
		case MissionTemplateType::TrainWorker:
		{
			MapArea* area = areasFactory->getMapArea(mission->area);
			return new TrainWorkerExecutor(mission, status, area);
		}
		case MissionTemplateType::HotelRoom:
		{
			MapArea* area = areasFactory->getMapArea(mission->area);
			Place* hotelRoom = area->getClosestPlaceTo(mission->startPosition);
			return new HotelRoomExecutor(mission, status, hotelRoom, area);
		}
		default:
		{
			log(string("Cannot create mission executor. unknown mission template type: ").append(to_string((int)mission->templateType)).append(" Mission ID: ").append(to_string(mission->id)));
			return NULL;
		}
	}
}