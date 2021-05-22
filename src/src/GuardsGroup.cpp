#include "Main.h";

GuardsGroup::GuardsGroup(Vector3 defensePosition, float radius, bool shouldTolerate)
{
	this->defensePosition = defensePosition;
	this->radius = radius;
	this->shouldTolerate = shouldTolerate;
}

void GuardsGroup::add(Ped ped, IdlingModifier idlingModifier)
{
	RoutineParams routineParams;
	add(ped, idlingModifier, routineParams);
}

void GuardsGroup::add(Ped ped, IdlingModifier idlingModifier, RoutineParams routineParams)
{
	vector<GenericGuardingBehavior*>::iterator itr = guards.begin();
	while (itr != guards.end())
	{
		if ((*itr)->ped() == ped)
		{
			return;
		}
		itr++;
	}

	GenericGuardingBehavior* guard = new GenericGuardingBehavior(ped, defensePosition, radius, idlingModifier, routineParams, &bodiesFound, shouldTolerate);
	guards.push_back(guard);
}

vector<Ped>* GuardsGroup::peds()
{
	vector<Ped>* peds = new vector<Ped>;
	for (vector<GenericGuardingBehavior*>::iterator itr = guards.begin(); itr != guards.end(); itr++)
	{
		peds->push_back((*itr)->ped());
	}
	return peds;
}

void GuardsGroup::start(bool withBlips)
{
	for (vector<GenericGuardingBehavior*>::iterator itr = guards.begin(); itr != guards.end(); itr++)
	{
		(*itr)->start(withBlips);
		guardsModeSnapshot[(*itr)->ped()] = (*itr)->getMode();
	}
}

void GuardsGroup::stop()
{
	for (vector<GenericGuardingBehavior*>::iterator itr = guards.begin(); itr != guards.end(); itr++)
	{
		(*itr)->stop();
	}
}

void GuardsGroup::update()
{
	clearDeadGuards();
	if (guards.size() == 0)
	{
		stop();
		return;
	}

	TensionMode sharedTensionMode = detectSharedTensionMode();
	for (vector<GenericGuardingBehavior*>::iterator itr = guards.begin(); itr != guards.end(); itr++)
	{
		GenericGuardingBehavior* guard = (*itr);
	
		if (sharedTensionMode > TensionMode::Idle)
		{
			switch (sharedTensionMode)
			{
			case TensionMode::Alerted:
				guard->alert();
				break;
			case TensionMode::Search:
				guard->search();
				break;
			case TensionMode::Combat:
				guard->combat();
				break;
			}
		}
		
		guard->update();
		guardsModeSnapshot[guard->ped()] = guard->getMode();
	}
}

void GuardsGroup::alert()
{
	for (vector<GenericGuardingBehavior*>::iterator itr = guards.begin(); itr != guards.end(); itr++)
	{
		(*itr)->alert();
	}
}

void GuardsGroup::search()
{
	for (vector<GenericGuardingBehavior*>::iterator itr = guards.begin(); itr != guards.end(); itr++)
	{
		(*itr)->search();
	}
}

void GuardsGroup::combat()
{
	for (vector<GenericGuardingBehavior*>::iterator itr = guards.begin(); itr != guards.end(); itr++)
	{
		(*itr)->combat();
	}
}

void GuardsGroup::clearDeadGuards()
{
	//BaseMissionExecutor* ped;
	//vector<GenericGuardingBehavior*>::iterator itr = guards.begin();
	//while (itr != guards.end())
	//{
	//	(*itr)->update();
	//	Ped pedItr = (*itr)->ped();

	//	if (ENTITY::IS_ENTITY_DEAD((*itr)->ped()) || distanceBetweenEntities((*itr)->ped(), PLAYER::PLAYER_PED_ID()) > 250)
	//	{
	//		(*itr)->stop();
	//		itr = guards.erase(itr);
	//		ped->releaseEntitySafe(&pedItr);
	//	}
	//	else
	//	{
	//		itr++;
	//	}
	//}
}

void GuardsGroup::addBlips()
{
	for (vector<GenericGuardingBehavior*>::iterator itr = guards.begin(); itr != guards.end(); itr++)
	{
		(*itr)->addEnemyBlip();
	}
}

void GuardsGroup::removeBlips()
{
	for (vector<GenericGuardingBehavior*>::iterator itr = guards.begin(); itr != guards.end(); itr++)
	{
		(*itr)->removeBlip();
	}
}

TensionMode GuardsGroup::detectSharedTensionMode()
{
	TensionMode result = TensionMode::Idle;
	vector<GenericGuardingBehavior*>::iterator itr = guards.begin();
	while (itr != guards.end())
	{
		TensionMode currMode = (*itr)->getMode();
		if (guardsModeSnapshot[(*itr)->ped()] != currMode && shouldModeBeShared(currMode) && currMode > result)
		{
			result = currMode;
		}
		itr++;
	}

	return result;
}

bool GuardsGroup::shouldModeBeShared(TensionMode mode)
{
	return (mode == TensionMode::Search || mode == TensionMode::Alerted || mode == TensionMode::Combat);
}