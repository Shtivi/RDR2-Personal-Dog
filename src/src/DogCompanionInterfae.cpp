#include "Main.h"

vector<const char*> ComplimentsMale = {
	"HORSE_COMPLIMENT_LOW_MALE",
	"HORSE_COMPLIMENT_HIGH_MALE",
	"HORSE_BIG_COMPLIMENT_LOW_MALE",
	"HORSE_BIG_COMPLIMENT_HIGH_MALE"
};

vector<const char*> ComplimentsFemale = {
	"HORSE_BIG_COMPLIMENT_HIGH_FEMALE",
	"HORSE_BIG_COMPLIMENT_LOW_FEMALE",
	"HORSE_COMPLIMENT_HIGH_FEMALE",
	"HORSE_COMPLIMENT_LOW_FEMALE"
};

DogCompanionInterface::DogCompanionInterface(Ped companion)
{
	this->companion = companion;
}

Ped DogCompanionInterface::getPed()
{
	return this->companion;
}

void DogCompanionInterface::stay()
{
	TASK::TASK_START_SCENARIO_IN_PLACE_HASH(getPed(), MISC::GET_HASH_KEY("WORLD_ANIMAL_DOG_SITTING"), -1, 1, 1, 0, 1);
}

void DogCompanionInterface::follow()
{
	FLOCK::SET_ANIMAL_TUNING_FLOAT_PARAM(companion, 165, 5);
	TASK::TASK_CLEAR_LOOK_AT(getPed());
	if (!PED::IS_PED_GROUP_MEMBER(companion, PED::GET_PED_GROUP_INDEX(player), 0))
	{
		//PED::SET_PED_AS_GROUP_MEMBER(companion, PED::GET_PED_GROUP_INDEX(player));
		Vector3 coords = entityPos(player);
		TASK::CLEAR_PED_TASKS_IMMEDIATELY(companion, 1, 1);
		//TASK::TASK_FOLLOW_NAV_MESH_TO_COORD(companion, coords.x, coords.y, coords.z, 3, -1, 2, false, 0);
		TASK::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(companion, player, 0, 0, 0, 3, 100000, 5, false, 0, 0, 0, 0, 0);
	}
	else
	{
		TASK::CLEAR_PED_TASKS(companion, 1, 1);
		//TASK::CLEAR_PED_TASKS_IMMEDIATELY(companion, 1, 1);
		//TASK::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(companion, player, 0, 0, 0, 3, 100000, 5, false, 0, 0, 0, 0);
	}
}

void DogCompanionInterface::fetch(Entity entity, int timeout)
{
	TASK::TASK_LOOK_AT_ENTITY(getPed(), player, 12000, 0, 0, 0);
	Vector3 entityCoords = entityPos(entity);
	Object seq;
	TASK::OPEN_SEQUENCE_TASK(&seq);
	TASK::TASK_FOLLOW_NAV_MESH_TO_COORD(0, entityCoords.x, entityCoords.y, entityCoords.z, 3, timeout, 1, false, 0);
	TASK::TASK_START_SCENARIO_IN_PLACE_HASH(0, MISC::GET_HASH_KEY("WORLD_ANIMAL_DOG_SNIFFING_GROUND"), 1500, 1, 1, 0, 1);
	TASK::TASK_START_SCENARIO_IN_PLACE_HASH(0, MISC::GET_HASH_KEY("WORLD_ANIMAL_DOG_BARKING_UP"), 10000, 1, 1, 0, 1);
	TASK::CLOSE_SEQUENCE_TASK(seq);
	TASK::TASK_PERFORM_SEQUENCE(getPed(), seq);
}

AsyncCompanionTask* DogCompanionInterface::warn(Entity entity, int timeout)
{
	return new WarnAsyncTask(getPed(), entity, timeout);
}

AsyncCompanionTask* DogCompanionInterface::warnSnake(Entity entity, int timeout)
{
	return new WarnAsyncTask(getPed(), entity, timeout, "WORLD_ANIMAL_DOG_BARKING_GROUND");
}

void DogCompanionInterface::dismiss()
{
	playVocalization("WHINE");
	TASK::CLEAR_PED_TASKS(getPed(), 1, 1);
}

void DogCompanionInterface::getPraised(Entity ped)
{
	const char* speech = ComplimentsMale.at(rndInt(0, ComplimentsMale.size()));
	if (!PED::IS_PED_MALE(getPed()))
	{
		speech = ComplimentsFemale.at(rndInt(0, ComplimentsFemale.size()));;
	}
	playAmbientSpeech(ped, (char*)speech);

	if (this->isSittingDown())
	{
		playVocalization("BARK_HAPPY");
	}
	else
	{
		TASK::CLEAR_PED_TASKS(getPed(), 1, 1);
		Object seq;
		TASK::OPEN_SEQUENCE_TASK(&seq);
		TASK::TASK_LOOK_AT_ENTITY(0, player, 2000, 0, 0, 0);
		TASK::TASK_START_SCENARIO_IN_PLACE_HASH(0, MISC::GET_HASH_KEY("WORLD_ANIMAL_DOG_BARKING_UP"), 2500, 1, 1, 0, 1);
		TASK::CLOSE_SEQUENCE_TASK(seq);
		TASK::TASK_PERFORM_SEQUENCE(getPed(), seq);
	}

}

void DogCompanionInterface::combat(Ped target)
{
	TASK::TASK_COMBAT_PED(companion, target, 0, 16);
}

void DogCompanionInterface::eat()
{
	int unk = TASK::_0x244430C13BA5258E(companion, 0, 1, 0);
	TASK::TASK_EAT(companion, unk, 0);
}

void DogCompanionInterface::beg(int duration, Entity lookAt)
{
	if (lookAt && ENTITY::DOES_ENTITY_EXIST(lookAt))
	{
		TASK::TASK_LOOK_AT_ENTITY(getPed(), lookAt, duration, 0, 0, 0);
	}
	TASK::CLEAR_PED_TASKS(getPed(), 1, 1);
	Object seq;
	TASK::OPEN_SEQUENCE_TASK(&seq);
	TASK::TASK_START_SCENARIO_IN_PLACE_HASH(0, MISC::GET_HASH_KEY("WORLD_ANIMAL_DOG_BEGGING"), duration, 1, 1, 0, 1);
	TASK::CLOSE_SEQUENCE_TASK(seq);
	TASK::TASK_PERFORM_SEQUENCE(getPed(), seq);
}

WaitOutsideInteriorAsyncTask* DogCompanionInterface::waitOutsideInterior()
{
	return new WaitOutsideInteriorAsyncTask(getPed(), player);
}

bool DogCompanionInterface::isEntityEatable(Entity entity)
{
	return
		ENTITY::GET_IS_ANIMAL(entity) &&
		!PED::_IS_THIS_MODEL_A_HORSE(ENTITY::GET_ENTITY_MODEL(findCarriedPedBy(player))) &&
		!isPedADog(entity) &&
		!ENTITY::_GET_IS_PREDATOR(entity) &&
		!MISC::_IS_PED_DECOMPOSED(entity);
}

void DogCompanionInterface::getPreyMeal(Ped from)
{
	Conversation* interaction = new Conversation();

	Vector3 companionCoords = entityPos(companion);
	Vector3 placementCoords = companionCoords + getForwardVector(companion) * 1.5;
	Entity meal = findCarriedPedBy(from);

	Object playerTasks;
	TASK::OPEN_SEQUENCE_TASK(&playerTasks);
	TASK::TASK_TURN_PED_TO_FACE_ENTITY(0, companion, 2000, 0, 0, 0);
	TASK::TASK_PLACE_CARRIED_ENTITY_AT_COORD(0, meal, placementCoords.x, placementCoords.y, placementCoords.z, 1073741824, 1); // TASK_PLACE_CARRIED_ENTITY_AT_COORD
	TASK::CLOSE_SEQUENCE_TASK(playerTasks);
	TASK::TASK_PERFORM_SEQUENCE(from, playerTasks);

	TASK::TASK_LOOK_AT_ENTITY(companion, meal, -1, 0, 0, 0);

	Stopwatch timeout;
	WAIT(200);
	timeout.start();
	while (TASK::GET_SEQUENCE_PROGRESS(from) > -1 && timeout.getElapsedSeconds() < 5)
	{
		WAIT(10);
	}

	if (timeout.getElapsedSeconds() >= 5)
	{
		return;
	}
	TASK::CLEAR_PED_TASKS(companion, 1, 1);

	playAmbientSpeech(from, PED::IS_PED_MALE(companion) ? "HORSE_BIG_COMPLIMENT_HIGH_MALE" : "HORSE_BIG_COMPLIMENT_HIGH_FEMALE");
	playVocalization("BARK_HAPPY");
	Vector3 mealCoords = entityPos(meal);
	TASK::TASK_GOTO_ENTITY_OFFSET_XY(companion, meal, 5000, 0, 0, 0, 1, 0);
	timeout.start();
	while (timeout.getElapsedSeconds() < 5 && distance(companion, meal) > 0.8)
	{
		WAIT(10);
	}

	if (timeout.getElapsedSeconds() >= 5)
	{
		return;
	}

	playAnimation(companion, "idle_a", "amb_creature_mammal@world_dog_eating_ground@idle_a", 15000, 2, -2, 1);
	PED::_SET_PED_QUALITY(meal, 0);
	ENTITY::SET_ENTITY_HEALTH(companion, ENTITY::GET_ENTITY_MAX_HEALTH(companion, 0), 0);
}

void DogCompanionInterface::playVocalization(const char* vocalization)
{
	invoke<Void>(0xEE066C7006C49C0A, companion, vocalization, 1);
}

bool DogCompanionInterface::isVocalizationPlaying()
{
	return AUDIO::IS_ANIMAL_VOCALIZATION_PLAYING(companion); // IS_ANIMAL_VOCALIZATION_PLAYING
}

bool DogCompanionInterface::isPerformingScenario()
{
	return TASK::GET_SCRIPT_TASK_STATUS(getPed(), 0x3B3A458F, 0) == 1;
}

bool DogCompanionInterface::isPaused()
{
	return TASK::GET_SCRIPT_TASK_STATUS(getPed(), 0x03C990EC, 0) == 1;
}

bool DogCompanionInterface::isSittingDown()
{
	return PED::IS_PED_USING_SCENARIO_HASH(getPed(), MISC::GET_HASH_KEY("WORLD_ANIMAL_DOG_SITTING"));
}

void DogCompanionInterface::setName(const char* name)
{
	PED::_SET_PED_PROMPT_NAME(getPed(), name);
}

bool DogCompanionInterface::isAgitated()
{
	return 
		PED::IS_PED_IN_MELEE_COMBAT(getPed()) || 
		PED::IS_PED_USING_SCENARIO_HASH(getPed(), MISC::GET_HASH_KEY("WORLD_ANIMAL_DOG_BARK_GROWL")) ||
		PED::IS_PED_USING_SCENARIO_HASH(getPed(), MISC::GET_HASH_KEY("WORLD_ANIMAL_DOG_BARKING_GROUND"));
}

bool DogCompanionInterface::isInCombat()
{
	return PED::IS_PED_IN_MELEE_COMBAT(getPed());
}

float DogCompanionInterface::getHealthRate()
{
	return (float)ENTITY::GET_ENTITY_HEALTH(companion) / ENTITY::GET_ENTITY_MAX_HEALTH(companion, 1);
}

bool DogCompanionInterface::isBegging()
{
	return TASK::GET_SEQUENCE_PROGRESS(getPed()) > -1 || PED::IS_PED_USING_SCENARIO_HASH(getPed(), MISC::GET_HASH_KEY("WORLD_ANIMAL_DOG_BEGGING"));
}

AsyncCompanionTask* DogCompanionInterface::track(Ped ped)
{
	if (ped == getPed())
	{
		return NULL;
	}

	return new TrackEntityAsyncTask(getPed(), ped, 50);
}

void DogCompanionInterface::hunt(Ped target, float timeout)
{
	Object seq;
	TASK::OPEN_SEQUENCE_TASK(&seq);
	if (distance(getPed(), target) > 25)
	{
		TASK::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(0, target, 0, 0, 0, 1, timeout, 25, 0, 0, 0, 0, 0, 0);
	}
	TASK::TASK_COMBAT_PED(0, target, 0, 16);
	TASK::CLOSE_SEQUENCE_TASK(seq);
	TASK::TASK_PERFORM_SEQUENCE(getPed(), seq);
}

void DogCompanionInterface::flee(Ped threat, int duration, float distance)
{
	TASK::TASK_FLEE_PED(getPed(), threat, 3, 0, distance, duration, 0);
}