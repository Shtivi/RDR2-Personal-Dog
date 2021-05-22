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
	AI::_0x524B54361229154F(getPed(), GAMEPLAY::GET_HASH_KEY("WORLD_ANIMAL_DOG_SITTING"), -1, 1, 1, 0, 1);
}

void DogCompanionInterface::follow()
{
	ANIMALTUNING::_0xCBDA22C87977244F(companion, 165, 5);
	AI::TASK_CLEAR_LOOK_AT(getPed());
	if (!PED::IS_PED_GROUP_MEMBER(companion, PED::GET_PED_GROUP_INDEX(player), 0))
	{
		PED::SET_PED_AS_GROUP_MEMBER(companion, PED::GET_PED_GROUP_INDEX(player));
		Vector3 coords = entityPos(player);
		AI::CLEAR_PED_TASKS_IMMEDIATELY(companion, 1, 1);
		AI::TASK_FOLLOW_NAV_MESH_TO_COORD(companion, coords.x, coords.y, coords.z, 3, -1, 5, false, 0);
	}
	else
	{
		AI::CLEAR_PED_TASKS_IMMEDIATELY(companion, 1, 1);

		AI::TASK_FOLLOW_TO_OFFSET_OF_ENTITY(companion, player, 0, 0, 0, 3, 100000, 5, false, 0, 0, 0, 0);
	}
}

void DogCompanionInterface::fetch(Entity entity, int timeout)
{
	AI::TASK_LOOK_AT_ENTITY(getPed(), player, 12000, 0, 0, 0);
	Vector3 entityCoords = entityPos(entity);
	Object seq;
	AI::OPEN_SEQUENCE_TASK(&seq);
	AI::TASK_FOLLOW_NAV_MESH_TO_COORD(0, entityCoords.x, entityCoords.y, entityCoords.z, 3, timeout, 1, false, 0);
	AI::_0x524B54361229154F(0, GAMEPLAY::GET_HASH_KEY("WORLD_ANIMAL_DOG_SNIFFING_GROUND"), 1500, 1, 1, 0, 1);
	AI::_0x524B54361229154F(0, GAMEPLAY::GET_HASH_KEY("WORLD_ANIMAL_DOG_BARKING_UP"), 10000, 1, 1, 0, 1);
	AI::CLOSE_SEQUENCE_TASK(seq);
	AI::TASK_PERFORM_SEQUENCE(getPed(), seq);
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
	AI::CLEAR_PED_TASKS(getPed(), 1, 1);
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
		AI::CLEAR_PED_TASKS(getPed(), 1, 1);
		Object seq;
		AI::OPEN_SEQUENCE_TASK(&seq);
		AI::TASK_LOOK_AT_ENTITY(0, player, 2000, 0, 0, 0);
		AI::_0x524B54361229154F(0, GAMEPLAY::GET_HASH_KEY("WORLD_ANIMAL_DOG_BARKING_UP"), 2500, 1, 1, 0, 1);
		AI::CLOSE_SEQUENCE_TASK(seq);
		AI::TASK_PERFORM_SEQUENCE(getPed(), seq);
	}

}

void DogCompanionInterface::combat(Ped target)
{
	AI::TASK_COMBAT_PED(companion, target, 0, 16);
}

void DogCompanionInterface::eat()
{
	int unk = AI::_0x244430C13BA5258E(companion, 0, 1, 0);
	AI::_0xBD7949BD07299672(companion, unk, 0);
}

void DogCompanionInterface::beg(int duration, Entity lookAt)
{
	if (lookAt && ENTITY::DOES_ENTITY_EXIST(lookAt))
	{
		AI::TASK_LOOK_AT_ENTITY(getPed(), lookAt, duration, 0, 0, 0);
	}
	AI::CLEAR_PED_TASKS(getPed(), 1, 1);
	Object seq;
	AI::OPEN_SEQUENCE_TASK(&seq);
	AI::_0x524B54361229154F(0, GAMEPLAY::GET_HASH_KEY("WORLD_ANIMAL_DOG_BEGGING"), duration, 1, 1, 0, 1);
	AI::CLOSE_SEQUENCE_TASK(seq);
	AI::TASK_PERFORM_SEQUENCE(getPed(), seq);
}

WaitOutsideInteriorAsyncTask* DogCompanionInterface::waitOutsideInterior()
{
	return new WaitOutsideInteriorAsyncTask(getPed(), player);
}

bool DogCompanionInterface::isEntityEatable(Entity entity)
{
	return
		ENTITY::_0x9A100F1CF4546629(entity) && // IS_PED_ANIMAL
		!PED::_0x772A1969F649E902(ENTITY::GET_ENTITY_MODEL(findCarriedPedBy(player))) && // IS_THIS_MODEL_HORSE
		!isPedADog(entity) &&
		!ENTITY::_0x5594AFE9DE0C01B7(entity) && // _GET_IS_PREDATOR
		!GAMEPLAY::_0x5170DDA6D63ACAAA(entity); // _IS_PED_DECOMPOSED
}

void DogCompanionInterface::getPreyMeal(Ped from)
{
	Conversation* interaction = new Conversation();

	Vector3 companionCoords = entityPos(companion);
	Vector3 placementCoords = companionCoords + getForwardVector(companion) * 1.5;
	Entity meal = findCarriedPedBy(from);

	Object playerTasks;
	AI::OPEN_SEQUENCE_TASK(&playerTasks);
	AI::TASK_TURN_PED_TO_FACE_ENTITY(0, companion, 2000, 0, 0, 0);
	AI::_0xC7F0B43DCDC57E3D(0, meal, placementCoords.x, placementCoords.y, placementCoords.z, 1073741824, 1); // TASK_PLACE_CARRIED_ENTITY_AT_COORD
	AI::CLOSE_SEQUENCE_TASK(playerTasks);
	AI::TASK_PERFORM_SEQUENCE(from, playerTasks);

	AI::TASK_LOOK_AT_ENTITY(companion, meal, -1, 0, 0, 0);

	Stopwatch timeout;
	WAIT(200);
	timeout.start();
	while (AI::GET_SEQUENCE_PROGRESS(from) > -1 && timeout.getElapsedSeconds() < 5)
	{
		WAIT(10);
	}

	if (timeout.getElapsedSeconds() >= 5)
	{
		return;
	}
	playAmbientSpeech(from, PED::IS_PED_MALE(companion) ? "HORSE_BIG_COMPLIMENT_HIGH_MALE" : "HORSE_BIG_COMPLIMENT_HIGH_FEMALE");
	playVocalization("BARK_HAPPY");
	Vector3 mealCoords = entityPos(meal);
	AI::TASK_GOTO_ENTITY_OFFSET_XY(companion, meal, 5000, 0, 0, 0, 1, 0);
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
	PED::_0xCE6B874286D640BB(meal, 0);
	ENTITY::SET_ENTITY_HEALTH(companion, ENTITY::GET_ENTITY_MAX_HEALTH(companion, 0), 0);
}

void DogCompanionInterface::playVocalization(const char* vocalization)
{
	invoke<Void>(0xEE066C7006C49C0A, companion, vocalization, 1);
}

bool DogCompanionInterface::isVocalizationPlaying()
{
	return AUDIO::_0xC265DF9FB44A9FBD(companion); // IS_ANIMAL_VOCALIZATION_PLAYING
}

bool DogCompanionInterface::isPerformingScenario()
{
	return AI::GET_SCRIPT_TASK_STATUS(getPed(), 0x3B3A458F, 0) == 1;
}

bool DogCompanionInterface::isPaused()
{
	return AI::GET_SCRIPT_TASK_STATUS(getPed(), 0x03C990EC, 0) == 1;
}

bool DogCompanionInterface::isSittingDown()
{
	return PED::_0x34D6AC1157C8226C(getPed(), GAMEPLAY::GET_HASH_KEY("WORLD_ANIMAL_DOG_SITTING"));
}

void DogCompanionInterface::setName(const char* name)
{
	PED::_0x4A48B6E03BABB4AC(getPed(), (Any*)name); // _SET_PED_PROMPT_NAME
}

bool DogCompanionInterface::isAgitated()
{
	return 
		PED::IS_PED_IN_MELEE_COMBAT(getPed()) || 
		PED::_0x34D6AC1157C8226C(getPed(), GAMEPLAY::GET_HASH_KEY("WORLD_ANIMAL_DOG_BARK_GROWL")) || 
		PED::_0x34D6AC1157C8226C(getPed(), GAMEPLAY::GET_HASH_KEY("WORLD_ANIMAL_DOG_BARKING_GROUND"));
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
	return AI::GET_SEQUENCE_PROGRESS(getPed()) > -1 || PED::_0x34D6AC1157C8226C(getPed(), GAMEPLAY::GET_HASH_KEY("WORLD_ANIMAL_DOG_BEGGING"));
}

AsyncCompanionTask* DogCompanionInterface::track(Ped ped)
{
	if (ped == getPed())
	{
		return NULL;
	}

	return new TrackEntityAsyncTask(getPed(), ped, 50);
}