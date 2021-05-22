#include "Main.h"

const float SEEING_RANGE = 65;
const float AIMING_TOLERANCE_DISTANCE = 10;
const float HEARING_RANGE = 100;

RidersGroup::RidersGroup(Ped leader, int distance, int warningTimeTolerance, bool persistLeaderBlip)
{
	setSafeDistance(distance);
	setWarningTimeTolerance(warningTimeTolerance);

	wasAlerted = false;

	int group = PED::GET_PED_GROUP_INDEX(leader);
	if (!PED::DOES_GROUP_EXIST(group))
	{
		group = PED::CREATE_GROUP(0);
		PED::SET_GROUP_FORMATION(group, 1);
	}
	setLeader(new RiderAI(leader, persistLeaderBlip), group);
	enterIdleMode();
}

int RidersGroup::getWarningTimeTolerance()
{
	return warningTimeTolerance;
}

void RidersGroup::setWarningTimeTolerance(int time)
{
	warningTimeTolerance = time;
}

void RidersGroup::setSafeDistance(int distance)
{
	safeDistance = distance;
}

int RidersGroup::getSafeDistance()
{
	return safeDistance;
}


AlertnessStatus RidersGroup::getAlertnessStatus()
{
	return status;
}

void RidersGroup::setAlertnessStatus(AlertnessStatus alertnessStatus)
{
	this->status = alertnessStatus;
}

void RidersGroup::addRider(Ped rider)
{
	PED::SET_PED_AS_GROUP_MEMBER(rider, getGroupId());
	if (PED::IS_PED_ON_MOUNT(rider))
	{
		PED::SET_PED_AS_GROUP_MEMBER(PED::GET_MOUNT(rider), getGroupId());
	}

	addPed(rider);
}

void RidersGroup::addPassanger(Ped passanger)
{
	PED::SET_PED_AS_GROUP_MEMBER(passanger, getGroupId());
	passangers.push_back(new PassangerAI(passanger));
}

void RidersGroup::update()
{
	if (members.size() == 0)
	{
		return;
	}


	//if (!leader || !leader->isAlive())
	//{
	//	setLeader(members[0]);
	//}

	Vector3 playerPos = entityPos(player);
	float leaderDistance = distance(playerPos, leader->ped());


	//stringstream s;
	//s << (int)status << ", " << (float)stopwatch.getElapsedSeconds() << ", " << leaderDistance;
	//debug(s.str().c_str());


	if (status != AlertnessStatus::Combat && isGroupThreatenedByPlayer())
	{
		enterCombatMode(player);
	}

	switch (status)
	{
		case AlertnessStatus::Idle: 
		{
			if (leaderDistance <= safeDistance)
			{
				enterWarningMode(player);
			}
			break;
		}
		case AlertnessStatus::Warn: 
		{
			if (leaderDistance >= 1.5 * safeDistance)
			{
				enterIdleMode();
			}
			else if (stopwatch.getElapsedSeconds() >= warningTimeTolerance)
			{
				enterThreateningMod(player);
			}
			break;
		}
		case AlertnessStatus::Threaten:
		{
			if (leaderDistance >= 1.5 * safeDistance)
			{
				enterAlertedMode();
			}
			else if (stopwatch.getElapsedSeconds() >= warningTimeTolerance)
			{
				enterCombatMode(player);
			}
			break;
		}
		case AlertnessStatus::Alerted:
		{
			if (leaderDistance <= safeDistance)
			{
				enterThreateningMod(player);
			}
			break;
		}
		case AlertnessStatus::Combat:
		{
			if (areGuardsDead())
			{
				enterFleeMode(player);
			}
			else if (distanceTo(playerPos) > SEEING_RANGE)
			{
				enterAlertedMode();
			} 
			break;
		}
	}

	updatePassangers();

	//releaseUnnecessaryEntities();
}

void RidersGroup::showEnemyBlips()
{
	for (vector<RiderAI*>::iterator itr = members.begin(); itr != members.end(); itr++)
	{
		(*itr)->addEnemyBlip();
	}
}

void RidersGroup::removeEnemyBlips()
{
	for (vector<RiderAI*>::iterator itr = members.begin(); itr != members.end(); itr++)
	{
		(*itr)->removeBlip();
	}
}

int RidersGroup::getGroupId()
{
	return PED::GET_PED_GROUP_INDEX(leader->ped());
}

void RidersGroup::addPed(Ped ped)
{
	PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);
	addPed(new RiderAI(ped));
}

void RidersGroup::addPed(RiderAI* ped)
{
	if (!contains(ped->ped()))
	{
		members.push_back(ped);
	}
}

void RidersGroup::setLeader(RiderAI* leader, int groupId)
{
	PED::SET_PED_AS_GROUP_LEADER(leader->ped(), groupId, 1);
	addPed(leader);
	//if (PED::IS_PED_ON_MOUNT(leader->ped()))
	//{
	//	PED::SET_PED_AS_GROUP_MEMBER(PED::GET_MOUNT(leader->ped()), groupId);
	//}
	this->leader = leader;
}

RiderAI* RidersGroup::findClosestMemberTo(Vector3 location)
{
	RiderAI* best = leader;
	float bestDistance = distance(location, leader->ped());

	for (vector<RiderAI*>::iterator itr = members.begin(); itr != members.end(); itr++)
	{
		float currDistance = distance(location, (*itr)->ped());
		if (currDistance <= bestDistance && (*itr)->canShoot())
		{
			bestDistance = currDistance;
			best = *itr;
		}
	}

	return best;
}

void RidersGroup::releaseUnnecessaryEntities()
{
	vector<RiderAI*> membersToDelete;
	for (vector<RiderAI*>::iterator itr = members.begin(); itr != members.end(); itr++)
	{
		RiderAI* curr = *itr;
		if (!curr->isAlive())
		{
			membersToDelete.push_back(curr);
			curr->removeBlip();
		}
	}

	for (vector<RiderAI*>::iterator itr = membersToDelete.begin(); itr != members.end(); itr++)
	{
		members.erase(itr);
	}
}

bool RidersGroup::isGroupThreatenedByPlayer()
{
	float leaderDistance = distance(entityPos(player), leader->ped());
	for (vector<RiderAI*>::iterator itr = members.begin(); itr != members.end(); itr++)
	{
		RiderAI* curr = *itr;
		if (curr->isThreatenedByPlayer(SEEING_RANGE, HEARING_RANGE))
		{
			return true;
		}
	}

	//for (vector<PassangerAI*>::iterator itr = passangers.begin(); itr != passangers.end(); itr++)
	//{
	//	PassangerAI* curr = *itr;
	//	if (curr->isThreatenedByPlayer(SEEING_RANGE, HEARING_RANGE))
	//	{
	//		return true;
	//	}
	//}

	return false;
}

bool RidersGroup::contains(Ped ped)
{
	for (vector<RiderAI*>::iterator itr = members.begin(); itr != members.end(); itr++)
	{
		if ((*itr)->ped() == ped) {
			return true;
		}
	}

	return false;
}

float RidersGroup::distanceTo(Vector3 location)
{
	RiderAI* closest = findClosestMemberTo(location);
	if (!closest)
	{
		return 99999;
	}
	
	return distance(location, closest->ped());
}

void RidersGroup::updatePassangers()
{
	for (vector<PassangerAI*>::iterator itr = passangers.begin(); itr != passangers.end(); itr++)
	{
		(*itr)->update();
	}
}

bool RidersGroup::areGuardsDead()
{
	for (vector<RiderAI*>::iterator itr = members.begin(); itr != members.end(); itr++)
	{
		if ((*itr)->isAlive()) {
			return false;
		}
	}

	return true;
}

void RidersGroup::enterIdleMode()
{
	setAlertnessStatus(AlertnessStatus::Idle);
	leader->ride();
	removeEnemyBlips();
}

void RidersGroup::enterWarningMode(Ped suspect)
{
	setAlertnessStatus(AlertnessStatus::Warn);
	stopwatch.start();
	removeEnemyBlips();

	RiderAI* guard = findClosestMemberTo(entityPos(suspect));
	if (!guard)
	{
		return;
	}

	guard->warn(suspect);
}

void RidersGroup::enterThreateningMod(Ped suspect)
{
	setAlertnessStatus(AlertnessStatus::Threaten);
	stopwatch.start();
	showEnemyBlips();

	RiderAI* guard = findClosestMemberTo(entityPos(suspect));
	if (!guard)
	{
		return;
	}

	if (wasAlerted)
	{
		guard->threatenEscalated(suspect);
	}
	else 
	{
		guard->threaten(suspect);
	}
}

void RidersGroup::enterAlertedMode()
{
	setAlertnessStatus(AlertnessStatus::Alerted);
	wasAlerted = true;
	stopwatch.stop();
	removeEnemyBlips();

	for (vector<RiderAI*>::iterator itr = members.begin(); itr != members.end(); itr++)
	{
		AI::CLEAR_PED_TASKS((*itr)->ped(), 1, 1);
	}

	leader->ride(5);
}

void RidersGroup::enterCombatMode(Ped ped)
{
	setAlertnessStatus(AlertnessStatus::Combat);
	toleratePlayer = false;
	showEnemyBlips();

	for (vector<RiderAI*>::iterator itr = members.begin(); itr != members.end(); itr++)
	{
		(*itr)->fight(ped);
	}

	for (vector<PassangerAI*>::iterator itr = passangers.begin(); itr != passangers.end(); itr++)
	{
		(*itr)->cower(ped);
	}
}

void RidersGroup::enterFleeMode(Ped enemy)
{
	setAlertnessStatus(AlertnessStatus::Flee);

	for (vector<PassangerAI*>::iterator itr = passangers.begin(); itr != passangers.end(); itr++)
	{
		(*itr)->flee(enemy);
	}
}