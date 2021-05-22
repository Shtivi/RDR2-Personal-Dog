#include "Main.h"

vector<const char*> CAMPFIRE_SEATED_SCENARIOS = {
	"PROP_CAMP_FIRE_SEATED",
	"PROP_CAMP_FIRE_SEATED_TEND_FIRE",

};

CampingSite::CampingSite(Vector3 location, int defensiveRadius, int tolerationTime, CampVariant variant)
{
	this->location = location;
	this->defensiveRadius = defensiveRadius;
	this->tolerationTime = tolerationTime;
	this->alertnessStatus = CampAlertnessStatus::Idle;
	this->variant = variant;
	createPropset();
}

void CampingSite::play()
{
	enterIdleMode(true);
}

void CampingSite::update()
{
	float playerDistance = distance(location, player);

	//stringstream s;
	//s << (int)alertnessStatus << " " << playerDistance << " " << stopwatch.getElapsedSeconds();
	//debug(s.str());

	if (alertnessStatus < CampAlertnessStatus::Combat)
	{
		if (isGroupThreatened())
		{
			enterCombatMode();
		}
		else if (alertnessStatus != CampAlertnessStatus::Search)
		{
			updateEvents();
		}
	}

	switch (alertnessStatus)
	{
		case CampAlertnessStatus::Idle: {
			if (playerDistance <= defensiveRadius) 
			{
				alertedGuard = findAlertableGuard();
				if (alertedGuard)
				{
					enterSuspiciousMode();
				}
			}
			break;
		}
		case CampAlertnessStatus::Suspect: {
			if (playerDistance > defensiveRadius * 1.5)
			{
				enterIdleMode();
			}
			else if (stopwatch.getElapsedSeconds() >= tolerationTime || playerDistance <= defensiveRadius / 2)
			{
				if (!alertedGuard->isAlive())
				{
					alertedGuard = findAlertableGuard();
				}

				if (alertedGuard)
				{
					enterWarningMode();
				}
				else
				{
					enterIdleMode();
				}
			}
			break;
		}
		case CampAlertnessStatus::Warn: {
			if (playerDistance > defensiveRadius * 1.5)
			{
				enterAlertedMode();
			}
			else if (stopwatch.getElapsedSeconds() >= tolerationTime)
			{
				if (!alertedGuard->isAlive())
				{
					alertedGuard = findAlertableGuard();
				}

				if (alertedGuard)
				{
					enterCombatMode();
				}
				else
				{
					enterIdleMode();
				}
			}
			break;
		}
		case CampAlertnessStatus::Alerted: {
			if (playerDistance <= defensiveRadius)
			{
				alertedGuard = findAlertableGuard();
				if (alertedGuard)
				{
					enterCombatMode();
				}
			}
			break;
		}
		case CampAlertnessStatus::Search: {
			if (findAlertableGuard())
			{
				enterCombatMode();
			}

			if (stopwatch.getElapsedSeconds() > 30)
			{
				enterAlertedMode();
			}

			break;
		}
		case CampAlertnessStatus::Combat: {
			if (!anyGuardsLeft())
			{
				enterFleeMode();
			}
			break;
		}
	}
}

void CampingSite::addCamper(Ped ped)
{
	GuardAI* camper = new GuardAI(ped, 40, 55);
	campers.push_back(camper);
	allPeds.push_back(camper);
}

void CampingSite::addGuard(Ped ped, bool persistBlip)
{
	GuardAI* guard = new GuardAI(ped, 40, 55, persistBlip);
	guards.push_back(guard);
	allPeds.push_back(guard);
}

void CampingSite::showBlips()
{
	for (vector<GuardAI*>::iterator itr = guards.begin(); itr != guards.end(); itr++)
	{
		(*itr)->addEnemyBlip();
	}
}

void CampingSite::removeBlips()
{
	for (vector<GuardAI*>::iterator itr = guards.begin(); itr != guards.end(); itr++)
	{
		(*itr)->removeBlip();
	}
}

void CampingSite::createPropset()
{
	switch(variant)
	{
		case CampVariant::Campfire: {
			createPropsetCampfire();
			break;
		}
		case CampVariant::Tent:
		{
			createPropsetTent();
			break;
		}
		default:
		{
			createPropsetCampfire();
			break;
		}
	}
}

Object CampingSite::addProp(Object prop)
{
	if (ENTITY::IS_ENTITY_A_PED(prop))
	{
		PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(prop, true);
	}

	props.push_back(prop);
	return prop;
}

const char* CampingSite::generateSeatedScenario()
{
	return CAMPFIRE_SEATED_SCENARIOS.at(rndInt(0, CAMPFIRE_SEATED_SCENARIOS.size()));
}


void CampingSite::releaseUnnecessaryEntities()
{

}

void CampingSite::updateEvents()
{
	for (vector<GuardAI*>::iterator itr = allPeds.begin(); itr != allPeds.end(); itr++)
	{
		GuardAI* guard = *itr;
		DangerousEvent detectedEvent = guard->detectEventsAround();
		if (detectedEvent.eventType == DangerousEvents::None)
		{
			continue;
		}

		if (detectedEvent.eventType == DangerousEvents::DeadBody || detectedEvent.eventType == DangerousEvents::ShotsHeard)
		{
			enterSearchMode();
		}
	}
}

void CampingSite::cleanup()
{
	for (vector<GuardAI*>::iterator itr = allPeds.begin(); itr != allPeds.end(); itr++)
	{
		Ped entity = (*itr)->ped();
		ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&entity);
	}

	for (vector<Entity>::iterator itr = props.begin(); itr != props.end(); itr++)
	{
		Ped entity = (*itr);
		ENTITY::SET_OBJECT_AS_NO_LONGER_NEEDED(&entity);
	}
}

bool CampingSite::anyGuardsLeft()
{
	for (vector<GuardAI*>::iterator itr = guards.begin(); itr != guards.end(); itr++)
	{
		if ((*itr)->isAlive())
		{
			return true;
		}
	}

	return false;
}

GuardAI* CampingSite::findAlertableGuard()
{
	GuardAI* best = NULL;
	float bestDistance = 9999;

	for (vector<GuardAI*>::iterator itr = guards.begin(); itr != guards.end(); itr++)
	{
		GuardAI* guard = *itr;

		if (guard->canDetect(player))
		{
			float currDistance = distanceBetweenEntities(guard->ped(), player);
			if (currDistance < bestDistance)
			{
				best = guard;
				bestDistance = currDistance;
			}
		}
	}

	return best;
}

bool CampingSite::isGroupThreatened()
{
	for (vector<GuardAI*>::iterator itr = allPeds.begin(); itr != allPeds.end(); itr++)
	{
		GuardAI* curr = *itr;
		if (curr->isThreatenedByPlayer())
		{
			return true;
		}
	}

	return false;
}

void CampingSite::enterIdleMode(bool jumpIntoScenario)
{
	alertnessStatus = CampAlertnessStatus::Idle;
	alertedGuard = NULL;
	removeBlips();

	vector<int> availableIdlePoints = idleScenarios;
	for (vector<GuardAI*>::iterator itr = campers.begin(); itr != campers.end(); itr++)
	{
		GuardAI* curr = *itr;
		
		if (!availableIdlePoints.size()) 
		{
			break;
		}
		vector<int>::iterator scenarioPoint = availableIdlePoints.begin();
		curr->useScenarioPoint(*scenarioPoint, jumpIntoScenario);
		availableIdlePoints.erase(scenarioPoint);
	}

	vector<int> availableGuardingPosts = guardScenarios;
	for (vector<GuardAI*>::iterator guardItr = guards.begin(); guardItr != guards.end(); guardItr++)
	{
		GuardAI* curr = *guardItr;

		vector<int>::iterator scenarioPoint = availableGuardingPosts.begin();
		if (scenarioPoint == availableGuardingPosts.end())
		{
			if (!availableIdlePoints.size())
			{
				break;
			}
			scenarioPoint = availableIdlePoints.begin();
			curr->useScenarioPoint(*scenarioPoint, jumpIntoScenario);
			availableIdlePoints.erase(scenarioPoint);
			continue;
		}

		curr->useScenarioPoint(*scenarioPoint, jumpIntoScenario);
		availableGuardingPosts.erase(scenarioPoint);
	}
}

void CampingSite::enterSuspiciousMode()
{
	showBlips();
	alertnessStatus = CampAlertnessStatus::Suspect;
	stopwatch.start();
	alertedGuard->warn(player);
}

void CampingSite::enterWarningMode()
{
	showBlips();
	alertnessStatus = CampAlertnessStatus::Warn;
	stopwatch.start();
	for (vector<GuardAI*>::iterator itr = guards.begin(); itr != guards.end(); itr++)
	{
		GuardAI* guard = *itr;
		if (guard->canDetect(player))
		{
			guard->threaten(player, guard == alertedGuard);
		}
	}
}

void CampingSite::enterAlertedMode()
{
	enterIdleMode();
	alertnessStatus = CampAlertnessStatus::Alerted;
}

void CampingSite::enterSearchMode()
{
	alertnessStatus = CampAlertnessStatus::Search;
	stopwatch.start();

	for (vector<GuardAI*>::iterator itr = guards.begin(); itr != guards.end(); itr++)
	{
		GuardAI* guard = *itr;
		guard->search(entityPos(guard->ped()), defensiveRadius);
	}
}

void CampingSite::enterCombatMode()
{
	showBlips();
	alertnessStatus = CampAlertnessStatus::Combat;

	for (vector<GuardAI*>::iterator itr = guards.begin(); itr != guards.end(); itr++)
	{
		PED::_0xFE07FF6495D52E2A((*itr)->ped(), 0, 0, 0);
		(*itr)->fight(player);
	}

	for (vector<GuardAI*>::iterator itr = campers.begin(); itr != campers.end(); itr++)
	{
		PED::_0xFE07FF6495D52E2A((*itr)->ped(), 0, 0, 0);
		if (rndInt(0, 3) == 0)
		{
			(*itr)->flee(player);
		}
		else 
		{
			(*itr)->cower(player);
		}
	}
}

void CampingSite::enterFleeMode()
{
	removeBlips();
	alertnessStatus = CampAlertnessStatus::Flee;

	for (vector<GuardAI*>::iterator itr = allPeds.begin(); itr != allPeds.end(); itr++)
	{
		if ((*itr)->isAlive())
		{
			(*itr)->flee(player);
		}
	}
}

void CampingSite::createPropsetCampfire()
{
	Object campfire = addProp(createProp("p_campfire03x", location));
	ENTITY::FREEZE_ENTITY_POSITION(campfire, true);
	float heading = ENTITY::GET_ENTITY_HEADING(campfire);
	Vector3 forward = getForwardVector(campfire);
	Vector3 right = getRightVector(campfire);
	Object wagon = addProp(createProp("p_chuckwagon01b", location + forward * -5 + right * -6));

	addProp(createAmbientHorse(entityPos(wagon) + getForwardVector(wagon) * 6));
	addProp(createAmbientHorse(entityPos(wagon) + getForwardVector(wagon) * 6 + getRightVector(wagon) * 2));

	Object lootbox = addProp(createProp("p_boxmeddeposit01x", entityPos(wagon) + getForwardVector(wagon) * 4, 180));
	DECORATOR::DECOR_SET_INT(lootbox, "loot_item_type1", -1142684684); // money stack $50
	DECORATOR::DECOR_SET_INT(lootbox, "loot_item_amt1", 3);
	DECORATOR::DECOR_SET_INT(lootbox, "loot_item_model1", 0);
	DECORATOR::DECOR_SET_INT(lootbox, "loot_item_type2", 544283678); // platinum pocket watch
	DECORATOR::DECOR_SET_INT(lootbox, "loot_item_amt2", 1);
	DECORATOR::DECOR_SET_INT(lootbox, "loot_item_model2", 0);

	Vector3 guardLocation = location + forward * 7 + right * 5;
	guardScenarios.push_back(AI::_0x94B745CE41DB58A1(GAMEPLAY::GET_HASH_KEY("WORLD_HUMAN_GUARD_SCOUT"), guardLocation.x, guardLocation.y, guardLocation.z, 30, 0, -1, 1));

	guardLocation = location + forward * -5 + right * -8;
	guardScenarios.push_back(AI::_0x94B745CE41DB58A1(GAMEPLAY::GET_HASH_KEY("WORLD_HUMAN_GUARD_SCOUT"), guardLocation.x, guardLocation.y, guardLocation.z, 180, 0, -1, 1));

	guardLocation = location + forward * 5 + right * -8;
	guardScenarios.push_back(AI::_0x94B745CE41DB58A1(GAMEPLAY::GET_HASH_KEY("WORLD_HUMAN_GUARD_SCOUT"), guardLocation.x, guardLocation.y, guardLocation.z, 60, 0, -1, 1));

	Object stool2 = addProp(createProp("p_stool02x", location + forward * -2, 180));
	idleScenarios.push_back(AI::_0x794AB1379A74064D(stool2, GAMEPLAY::GET_HASH_KEY("PROP_HUMAN_SEAT_CHAIR_CIGAR"), 0, 0, 0.5, 180, 0, -1, 1)); // TASK_CREATE_SCENARIO_POINT_ATTACHED_TO_ENTITY

	Object stool1 = addProp(createProp("p_stool02x", location + forward * 2, 0));
	idleScenarios.push_back(AI::_0x794AB1379A74064D(stool1, GAMEPLAY::GET_HASH_KEY("PROP_CAMP_FIRE_SEATED"), 0, 0, 0.5, 180, 0, -1, 1)); // TASK_CREATE_SCENARIO_POINT_ATTACHED_TO_ENTITY

	Vector3 seatGroundOffset = forward * 2 + right * -2;
	idleScenarios.push_back(AI::_0x794AB1379A74064D(campfire, GAMEPLAY::GET_HASH_KEY("WORLD_CAMP_FIRE_SIT_GROUND"), seatGroundOffset.x, seatGroundOffset.y, 1, 270, 0, -1, 1)); // TASK_CREATE_SCENARIO_POINT_ATTACHED_TO_ENTITY

	Object stool3 = addProp(createProp("p_stool02x", location + forward + right * 2, -60));
	idleScenarios.push_back(AI::_0x794AB1379A74064D(stool3, GAMEPLAY::GET_HASH_KEY("PROP_CAMP_SEAT_CHAIR_STEW"), 0, 0, 0.5, 180, 0, -1, 1)); // TASK_CREATE_SCENARIO_POINT_ATTACHED_TO_ENTITY

	Object stool4 = addProp(createProp("p_stool02x", location - forward + right * 2, -120));
	idleScenarios.push_back(AI::_0x794AB1379A74064D(stool4, GAMEPLAY::GET_HASH_KEY("PROP_CAMP_FIRE_SEATED_TEND_FIRE"), 0, 0, 0.5, 180, 0, -1, 1)); // TASK_CREATE_SCENARIO_POINT_ATTACHED_TO_ENTITY
}

void CampingSite::createPropsetTent()
{

}