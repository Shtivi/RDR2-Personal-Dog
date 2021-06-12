#include "Main.h"

Prompt* createAccompanyPrompt()
{
	return new Prompt(DataFiles::Lang->get("interaction.accompany.promptLabel"), MISC::GET_HASH_KEY("INPUT_WHISTLE"));
}

Prompt* createStayPrompt()
{
	return new Prompt(DataFiles::Lang->get("interaction.stay.promptLabel"), MISC::GET_HASH_KEY("INPUT_CONTEXT_X"));
}

Prompt* createFollowPrompt()
{
	return new Prompt(DataFiles::Lang->get("interaction.follow.promptLabel"), MISC::GET_HASH_KEY("INPUT_CONTEXT_X"));
}

Prompt* createFeedPrompt()
{
	Prompt* prompt = new Prompt(DataFiles::Lang->get("interaction.feed.promptLabel"), MISC::GET_HASH_KEY("INPUT_INTERACT_OPTION1"));

	return prompt;
}

Prompt* createPraisePrompt()
{
	return new Prompt(DataFiles::Lang->get("interaction.praise.promptLabel"), MISC::GET_HASH_KEY("INPUT_INTERACT_OPTION1"));
}

Prompt* createDismissPrompt()
{
	return new Prompt(DataFiles::Lang->get("interaction.dismiss.promptLabel"), MISC::GET_HASH_KEY("INPUT_INTERACT_OPTION2"), PromptMode::SemiHold);
}

Prompt* createRenamePrompt()
{
	return new Prompt(DataFiles::Lang->get("interaction.rename.promptLabel"), MISC::GET_HASH_KEY("INPUT_LOOK_BEHIND"), PromptMode::SemiHold);
}

Prompt* createAttackPrompt(const char* dogName)
{
	return new Prompt(
		string(DataFiles::Lang->get("interaction.attack.promptLabel")).append(" ").append(dogName).c_str(), 
		MISC::GET_HASH_KEY((char*)ScriptSettings::get("AttackPromptControlName").c_str()), 
		PromptMode::SemiHold);
}

Prompt* createRetrieveDogPrompt(const char* dogName)
{
	return new Prompt(
		string(DataFiles::Lang->get("interaction.retrieve_dog.promptLabel")).append(" ").append(dogName).c_str(),
		MISC::GET_HASH_KEY("INPUT_LOOK_BEHIND"),
		PromptMode::SemiHold);
}

CompanionEngine::CompanionEngine()
{
	this->state = new RootState();
	this->state->coresUI = new ComapnionCoresUI(NULL);
}

RootState* CompanionEngine::getState()
{
	return this->state;
}

void CompanionEngine::start()
{
	if (!strlen(DataFiles::Dog->get("model")))
	{
		return;
	}

	vector<Ped> nearbyPeds = getNearbyPeds(player, 20);
	Ped dog = NULL;
	for (Ped current : nearbyPeds)
	{
		if (!ENTITY::IS_ENTITY_DEAD(current) && DECORATOR::DECOR_GET_INT(current, "SH_CMP_companion") == 1)
		{
			dog = current;
			break;
		}
	}
	if (!dog)
	{
		dog = spawnDog();
	}

	accompanyDog(dog);
	const char* name = DataFiles::Dog->get("name");
	if (name && strlen(name))
	{
		state->companionApi->setName(name);
	}
}

Ped CompanionEngine::spawnDog()
{
	Ped dog = createPed(DataFiles::Dog->getInt("model"), *getSafeCoordForPed(playerPos() + getForwardVector(player)));
	PED::_SET_PED_OUTFIT_PRESET(dog, DataFiles::Dog->getInt("preset_index"), 0); // _SET_PED_OUTFIT_PRESET
	return dog;
}

void CompanionEngine::update()
{
	int n = SCRIPTS::GET_NUMBER_OF_EVENTS(0);
	for (int i = 0; i < n; i++)
	{
		int eventType = SCRIPTS::GET_EVENT_AT_INDEX(0, i);
		if (eventType == MISC::GET_HASH_KEY("EVENT_PED_ANIMAL_INTERACTION"))
		{
			onAnimalInteraction(i);
		}
		else if (eventType == MISC::GET_HASH_KEY("EVENT_SHOT_FIRED_BULLET_IMPACT"))
		{
			onBulletImpact(i);
		}
		else if (eventType == MISC::GET_HASH_KEY("EVENT_PLAYER_PROMPT_TRIGGERED"))
		{
			onPromptTriggered(i);
		}
		else if (eventType == MISC::GET_HASH_KEY("EVENT_PED_WHISTLE"))
		{
			onWhistle(i);
		}
	}

	if (state->candidateDog && state->accompanyPrompt && state->accompanyPrompt->isActivatedByPlayer())
	{
		bondWithDog();
	}

	if (!state->companionDog || !state->companionApi)
	{
		if (!DataFiles::TutorialFlags->getBool("bond_1") && isPedADog(getPlayerTargetEntity()))
		{
			tutorial("bond_1");
		}

		return;
	}

	if (!ENTITY::DOES_ENTITY_EXIST(state->companionDog))
	{
		deleteBlipSafe(&state->companionBlip);
		accompanyDog(spawnDog());
		return;
	}

	if (ENTITY::IS_ENTITY_DEAD(state->companionDog))
	{
		onCompnaionDied();
		return;
	}

	if (!state->didPlayerHadControlLastFrame && 
		PLAYER::IS_PLAYER_CONTROL_ON(PLAYER::PLAYER_ID()) && 
		distance(player, state->companionDog) >= DataFiles::DogMeta->getInt("auto_retrieve.min_distance") &&
		!MISC::GET_MISSION_FLAG())
	{
		log("automatically retrieving dog");
		CompanionCommands::commandRetrieve(state);
	}

	float compaionDistanceToPlayer = distance(player, state->companionDog);
	state->isWithinWhistlingRange = compaionDistanceToPlayer < DataFiles::Dog->getInt("whistling_range");

	if (!PED::IS_PED_GROUP_MEMBER(state->companionDog, PED::GET_PED_GROUP_INDEX(player), 0) && compaionDistanceToPlayer < 30)
	{
		PED::SET_PED_AS_GROUP_MEMBER(state->companionDog, PED::GET_PED_GROUP_INDEX(player));
	}

	if (PAD::IS_CONTROL_JUST_RELEASED(0, MISC::GET_HASH_KEY("INPUT_WHISTLE")))
	{
		onWhistle(0);
	}

	float companionPreviousHealthRate = state->previousCompanionHealth / ENTITY::GET_ENTITY_MAX_HEALTH(state->companionDog, 1);
	float companionCurrentHealthRate = state->companionApi->getHealthRate();
	if (companionPreviousHealthRate > companionCurrentHealthRate &&
		companionPreviousHealthRate > DataFiles::DogMeta->getFloat("low_health_threshold") &&
		companionCurrentHealthRate < DataFiles::DogMeta->getFloat("low_health_threshold"))
	{
		showHelpText(DataFiles::Lang->get("tutorial.companion_low_health"));
	}

	if (state->currentTask)
	{
		if (state->currentTask->isRunning())
		{
			state->currentTask->update();
		}
		else if (state->currentTask->isDone())
		{
			state->currentTask = state->currentTask->getChainedTask();
		}
		else
		{
			state->currentTask = NULL;
		}
	}

	updateCompanionStats();
	scanCompanionSurrounding();
	updatePrompts();
	updateGUI();

	state->didPlayerHadControlLastFrame = PLAYER::IS_PLAYER_CONTROL_ON(PLAYER::PLAYER_ID());
}

void CompanionEngine::scanCompanionSurrounding()
{
	if (PED::IS_PED_IN_MELEE_COMBAT(player) && (!state->companionApi->isSittingDown() || !ScriptSettings::getBool("IgnorePlayerCombatWhenSittingDown")))
	{
		Ped playerTarget = PED::GET_CURRENT_TARGET_FOR_PED(player);
		if (playerTarget && 
			!PED::IS_PED_HOGTIED(playerTarget) &&
			!PED::_IS_PED_BEING_HOGTIED(playerTarget))
		{
			state->companionApi->combat(playerTarget);
		}
	}

	if (INTERIOR::GET_INTERIOR_FROM_ENTITY(player) && !state->companionApi->isSittingDown() && !ScriptSettings::getBool("AllowEnteringInteriors"))
	{
		state->currentTask = state->companionApi->waitOutsideInterior();
		tutorial("companion_wait_outside");
	}

	if (BUILTIN::TIMERA() <= DataFiles::DogMeta->getInt("scan_surrounding_interval") ||
		!state->isWithinWhistlingRange ||
		state->companionApi->isInCombat())
	{
		return;
	}
	BUILTIN::SETTIMERA(0);

	if (!state->calmTimer.isStarted() || state->calmTimer.getElapsedSeconds() >= DataFiles::DogMeta->getInt("calmTimeout"))
	{
		state->calmTimer.stop();
		Vector3 companionCoords = entityPos(state->companionDog);
		float detectionRadius = DataFiles::DogMeta->getInt("ambient_dangers_detection_radius");
		Ped predator = findClosestPredatorAround(player, detectionRadius);
		if (predator)
		{
			onPredatorDetected(predator);
		}
	}
}

void CompanionEngine::updateCompanionStats()
{
	if (!state->isWithinWhistlingRange)
	{
		return;
	}

	if (!state->statsTimer.isStarted())
	{
		state->statsTimer.start();
	}

	state->previousCompanionHealth = (float)ENTITY::GET_ENTITY_HEALTH(state->companionDog);

	int currentHealthCoreValue = DataFiles::Dog->getInt("health_core");
	if (!state->companionApi->isAgitated() && state->statsTimer.getElapsedSeconds() >= 1)
	{
		int currentHealth = ENTITY::GET_ENTITY_HEALTH(state->companionDog);
		int nextHealth = min(
			ENTITY::GET_ENTITY_MAX_HEALTH(state->companionDog, 1), 
			(int)(currentHealth + ((float)currentHealthCoreValue / 100) * DataFiles::DogMeta->getInt("health_regen_rate")));
		ENTITY::SET_ENTITY_HEALTH(state->companionDog, nextHealth, 0);
		state->statsTimer.start();
	}

	if (BUILTIN::TIMERB() > DataFiles::DogMeta->getInt("core_drain_rate") * 1000)
	{
		int nextHealthCoreValue = max(0, currentHealthCoreValue - DataFiles::DogMeta->getFloat("core_drain_impact") * 100);
		
		BUILTIN::SETTIMERB(0);
		DataFiles::Dog->set("health_core", nextHealthCoreValue);
		DataFiles::Dog->save();

		if (nextHealthCoreValue < DataFiles::DogMeta->getFloat("low_health_threshold") * 100 && 
			currentHealthCoreValue >= DataFiles::DogMeta->getFloat("low_health_threshold") * 100)
		{
			debug("low!!!!!!!!");
			showHelpText(DataFiles::Lang->get("tutorial.companion_low_health_core"), 10000);
		}
	}

	DECORATOR::DECOR_SET_INT(state->companionDog, "SH_CMP_health_core", DataFiles::Dog->getInt("health_core"));

}

void CompanionEngine::updatePrompts()
{
	if (state->stayPrompt->isActivatedByPlayer())
	{
		CompanionCommands::commandStay(state);
	}

	if (state->followPrompt->isActivatedByPlayer())
	{
		CompanionCommands::commandFollow(state);
	}

	if (state->praisePrompt->isActivatedByPlayer())
	{
		CompanionCommands::commandPraise(state);
	}

	if (distance(player, state->companionDog) < 2)
	{
		if (PED::IS_PED_CARRYING_SOMETHING(player) &&
			state->companionApi->isEntityEatable(findCarriedPedBy(player)))
		{
			if (!state->companionApi->isBegging())
			{
				state->companionApi->beg(12000, findCarriedPedBy(player));
			}
			state->feedPrompt->show();
			state->praisePrompt->hide();
			if (state->feedPrompt->isActivatedByPlayer())
			{
				state->companionApi->getPreyMeal(player);
				DataFiles::Dog->set("health_core", 100);
				DataFiles::Dog->save();
			}

			tutorial("companion_meal");
		}
		else
		{
			state->praisePrompt->show();
			state->feedPrompt->hide();
		}

		state->renamePrompt->show();
		if (state->renamePrompt->isActivatedByPlayer())
		{
			renameCompanion(getKeyboardInput("Enter Name", "Enter your dog's new name", "Dog", 20));
		}

		state->dismissPrompt->show();
		if (state->dismissPrompt->isActivatedByPlayer())
		{
			dismissCompanion();
		}
	}
	else
	{
		state->dismissPrompt->hide();
		state->feedPrompt->hide();
		state->renamePrompt->hide();
	}

	if (state->companionApi->isSittingDown())
	{
		state->followPrompt->show();
		state->stayPrompt->hide();
	}
	else
	{
		state->followPrompt->hide();
		state->stayPrompt->show();
	}

	if (ScriptSettings::getBool("AllowAttackPrompt") && state->isWithinWhistlingRange)
	{
		Entity targetEntity = getPlayerTargetEntity();
		if (targetEntity &&
			targetEntity != state->companionDog &&
			!ENTITY::IS_ENTITY_DEAD(targetEntity) &&
			distance(targetEntity, state->companionDog) <= DataFiles::DogMeta->getInt("unleash_range") &&
			(!INTERIOR::GET_INTERIOR_FROM_ENTITY(player) || ScriptSettings::getBool("AllowEnteringInteriors")) &&
			PED::GET_PED_RELATIONSHIP_GROUP_HASH(targetEntity) != MISC::GET_HASH_KEY("REL_GANG_DUTCHS") &&
			ENTITY::_GET_ENTITY_CAN_BE_DAMAGED(targetEntity) &&
			PED::IS_PED_ON_FOOT(targetEntity) &&
			!isPedADog(targetEntity) &&
			!PED::_IS_THIS_MODEL_A_HORSE(ENTITY::GET_ENTITY_MODEL(targetEntity)) &&
			(ENTITY::GET_IS_ANIMAL(targetEntity) || !ScriptSettings::getBool("AttackPromptAnimalsOnly")))
		{
			if (!state->attackPrompt || state->attackPrompt->getTargetEntity() != targetEntity)
			{
				state->attackPrompt = createAttackPrompt(DataFiles::Dog->get("name"));
			}
			state->attackPrompt->setTargetEntity(targetEntity);
			state->attackPrompt->show();
			if (state->attackPrompt->isActivatedByPlayer())
			{
				CompanionCommands::commandAttack(state, targetEntity);
			}
		}
		else if (state->attackPrompt)
		{
			state->attackPrompt->hide();
		}
	}

	if (!state->isWithinWhistlingRange && getPlayerSaddleHorse())
	{
		state->retrieveDogPrompt->setTargetEntity(getPlayerSaddleHorse());
		state->retrieveDogPrompt->show();
		if (state->retrieveDogPrompt->isActivatedByPlayer())
		{
			CompanionCommands::commandRetrieve(state);
		}
	}
	else
	{
		state->retrieveDogPrompt->hide();
	}

	bool isPlayerSpeaking = AUDIO::IS_ANY_SPEECH_PLAYING(player);
	state->praisePrompt->setIsEnabled(!isPlayerSpeaking);
	state->stayPrompt->setIsEnabled(!isPlayerSpeaking && !state->companionApi->isAgitated());
	state->dismissPrompt->setIsEnabled(!isPlayerSpeaking);
	state->followPrompt->setIsEnabled(!isPlayerSpeaking);
	if (state->attackPrompt)
	{
		state->attackPrompt->setIsEnabled(!isPlayerSpeaking && !state->companionApi->isInCombat());
	}

}

void CompanionEngine::updateGUI()
{
	if (!ScriptSettings::getBool("ShowCores"))
	{
		return;
	}

	if (state->companionApi->isAgitated())
	{
		state->coresUI->setCoreModifier(CompanionCoreModifiers::Agitated);
	}
	else
	{
		state->coresUI->setCoreModifier(CompanionCoreModifiers::None);
	}


	if (getPlayerTargetEntity() == state->companionDog || 
		state->companionApi->isAgitated() || 
		//DECORATOR::DECOR_GET_INT(state->companionDog, "SH_CMP_health_core") < DataFiles::DogMeta->getFloat("low_health_threshold") ||
		state->companionApi->getHealthRate() < DataFiles::DogMeta->getFloat("low_health_threshold"))
	{	
		state->coresUI->show();
		HUD::_SHOW_HORSE_CORES(true);
	}
	else
	{
		state->coresUI->hide();
		HUD::_SHOW_HORSE_CORES(false);
	}

	state->coresUI->update();
}

void CompanionEngine::onAnimalInteraction(int eventIndex)
{
	if (PED::IS_PED_ON_MOUNT(player) || state->companionDog)
	{
		return;
	}

	int arr[10];
	if (!SCRIPTS::GET_EVENT_DATA(0, eventIndex, (Any*)arr, 3))
	{
		log("WARN: animalInteraction event has no data");
		return;
	}

	int interactionType = arr[4];
	int animalPedId = arr[2];

	if (arr[0] == player && interactionType == MISC::GET_HASH_KEY("Interaction_Dog_Patting") && isPedADog(animalPedId))
	{
		state->candidateDog = animalPedId;
		if (state->accompanyPrompt)
		{
			state->accompanyPrompt->hide();
		}
		state->accompanyPrompt = createAccompanyPrompt();
		state->accompanyPrompt->setTargetEntity(state->candidateDog);
		state->accompanyPrompt->show();
		state->accompanyPrompt->setIsEnabled(true);
		tutorial("bond_2");
		log("bonding prompt available");
	}

}

void CompanionEngine::onPromptTriggered(int eventIndex)
{
	if (!state->companionDog || !state->companionApi || state->companionApi->isSittingDown())
	{
		return;
	}

	int eventData[30];
	if (!SCRIPTS::GET_EVENT_DATA(0, eventIndex, (Any*)eventData, 10))
	{
		return;
	}

	if (eventData[0] != 34) // PP_TRACK_ANIMAL
	{
		return;
	}

	Ped trackedEntity = eventData[4];
	if (ENTITY::_GET_IS_BIRD(trackedEntity))
	{
		return;
	}

	Vector3 pos = entityPos(trackedEntity);
	state->currentTask = state->companionApi->track(trackedEntity);
	log("tracking task triggered");
	tutorial("tracking");
}

void CompanionEngine::bondWithDog()
{
	log("animal bonded");
	state->accompanyPrompt->hide();
	DataFiles::Dog->set("preset_index", 1);
	DataFiles::Dog->set("bonding_level", 1);
	DataFiles::Dog->set("max_health", DataFiles::DogMeta->getInt("bonding.1.max_health"));
	DataFiles::Dog->set("will_attack_predators", 1);
	DataFiles::Dog->set("name", DataFiles::Lang->get("companion.defaultBlipName"));
	DataFiles::Dog->set("whistling_range", DataFiles::DogMeta->getInt("bonding.1.whistling_range"));
	DataFiles::Dog->set("fetch_range_min", 20);
	DataFiles::Dog->set("fetch_range_max", 70);
	DataFiles::Dog->set("health_core", 100);

	accompanyDog(state->candidateDog);
	state->candidateDog = NULL;
	DataFiles::Dog->set("model", (int )ENTITY::GET_ENTITY_MODEL(state->companionDog));
	DataFiles::Dog->save();
	tutorial("bond_3");
}

void CompanionEngine::accompanyDog(Ped dog)
{
	int group = PED::GET_PED_GROUP_INDEX(player);
	if (!PED::DOES_GROUP_EXIST(group))
	{
		group = PED::CREATE_GROUP(0);
		PED::SET_PED_AS_GROUP_LEADER(player, group, 1);
	}

	state->companionDog = dog;
	state->companionApi = new DogCompanionInterface(dog);

	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(dog, 1, 0);
	PED::SET_PED_AS_GROUP_MEMBER(dog, group);
	PED::SET_PED_CAN_BE_TARGETTED_BY_PLAYER(dog, PLAYER::PLAYER_ID(), false);
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(dog, PED::GET_PED_RELATIONSHIP_GROUP_HASH(player));
	DECORATOR::DECOR_SET_INT(dog, "SH_CMP_companion", 1);

	PED::SET_PED_CONFIG_FLAG(dog, 154, true); // PCF_UseFollowLeaderThreatResponse

	FLOCK::SET_ANIMAL_TUNING_BOOL_PARAM(dog, 44, 0); // ATB_StartFleeDecisionMakerDuringAlertedState
	FLOCK::SET_ANIMAL_TUNING_BOOL_PARAM(dog, 32, 1); // ATB_NeverRetreatFromMelee

	FLOCK::SET_ANIMAL_TUNING_FLOAT_PARAM(dog, 74, 1); // ATF_ThreatResponseCombatProbability
	FLOCK::SET_ANIMAL_TUNING_FLOAT_PARAM(dog, 80, 1); // ATF_ThreatResponseLoudNoiseFleeOrCombatRange
	FLOCK::SET_ANIMAL_TUNING_FLOAT_PARAM(dog, 115, 1); // ATF_ThreatResponseNoiseSmallCaliberFleeOrCombatRange
	FLOCK::SET_ANIMAL_TUNING_FLOAT_PARAM(dog, 117, 1); // ATF_ThreatResponseNoiseMediumCaliberFleeOrCombatRange
	FLOCK::SET_ANIMAL_TUNING_FLOAT_PARAM(dog, 119, 1); // ATF_ThreatResponseNoiseBigCaliberFleeOrCombatRange
	FLOCK::SET_ANIMAL_TUNING_FLOAT_PARAM(dog, 165, 5);
	ENTITY::SET_ENTITY_MAX_HEALTH(dog, DataFiles::Dog->getInt("max_health"));
	ENTITY::SET_ENTITY_HEALTH(dog, ENTITY::GET_ENTITY_MAX_HEALTH(dog, 0), 0);
	DECORATOR::DECOR_SET_INT(state->companionDog, "SH_CMP_health_core", DataFiles::Dog->getInt("health_core"));

	Blip dogBlip = createBlip(dog, 0x19365607);
	state->companionBlip = dogBlip;
	setBlipLabel(dogBlip, DataFiles::Lang->get("companion.defaultBlipName"));
	MAP::BLIP_ADD_MODIFIER(dogBlip, 0xB93E613);

	PED::_SET_PED_SCENT(dog, 0);

	BUILTIN::SETTIMERA(0);

	state->stayPrompt = createStayPrompt();
	state->stayPrompt->setTargetEntity(state->companionDog);
	state->stayPrompt->show();

	state->praisePrompt = createPraisePrompt();
	state->praisePrompt->setTargetEntity(state->companionDog);
	state->praisePrompt->show();

	state->feedPrompt = createFeedPrompt();
	state->feedPrompt->setTargetEntity(state->companionDog);
	state->feedPrompt->hide();

	state->dismissPrompt = createDismissPrompt();
	state->dismissPrompt->setTargetEntity(state->companionDog);
	state->dismissPrompt->hide();

	state->followPrompt = createFollowPrompt();
	state->followPrompt->setTargetEntity(state->companionDog);
	state->followPrompt->hide();

	state->renamePrompt = createRenamePrompt();
	state->renamePrompt->setTargetEntity(state->companionDog);
	state->renamePrompt->hide();
	
	state->retrieveDogPrompt = createRetrieveDogPrompt(DataFiles::Dog->get("name"));
	state->retrieveDogPrompt->hide();

	state->previousCompanionHealth = (float)ENTITY::GET_ENTITY_HEALTH(state->companionDog);
	this->state->coresUI->setPed(dog);
}

void CompanionEngine::onBulletImpact(int eventIndex)
{
	if (!state->companionDog)
	{
		return;
	}

	if (state->companionApi && 
		state->companionApi->isPerformingScenario() && 
		state->isWithinWhistlingRange)
	{
		return;
	}

	int arr[1];
	if (!SCRIPTS::GET_EVENT_DATA(0, eventIndex, (Any*)arr, 1))
	{
		return;
	}
	
	int entityId = arr[0];
	if (ENTITY::IS_ENTITY_DEAD(entityId) &&
		ENTITY::GET_IS_ANIMAL(entityId) &&
		!PED::_IS_THIS_MODEL_A_HORSE(ENTITY::GET_ENTITY_MODEL(entityId)) &&
		!ENTITY::_GET_IS_PREDATOR(entityId) &&
		PED::GET_PED_SOURCE_OF_DEATH(entityId) == player)
	{
		float distanceToAnimal = distance(player, entityId);
		if (distanceToAnimal < DataFiles::Dog->getInt("fetch_range_min") || distanceToAnimal > DataFiles::Dog->getInt("fetch_range_max"))
		{
			return;
		}

		if (ScriptSettings::getBool("MarkHuntedAnimals"))
		{
			state->companionApi->fetch(entityId);
			tutorial("fetch");
		}
	}
}

void CompanionEngine::onWhistle(int eventIndex)
{
	if (!state->companionDog)
	{
		return;
	}

	int arr[10];
	if (!SCRIPTS::GET_EVENT_DATA(0, eventIndex, (Any*)arr, 2))
	{
		return;
	}

	if (arr[0] != player)
	{
		return;
	}

	if (state->isWithinWhistlingRange)
	{
		if (INTERIOR::GET_INTERIOR_FROM_ENTITY(player) && !ScriptSettings::getBool("AllowEnteringInteriors"))
		{
			return;
		}

		CompanionCommands::triggerFollow(this->state);
	}
	else
	{
		showHelpText(DataFiles::Lang->get("tutorial.companion_out_of_whistling_range"));
	}
}

void CompanionEngine::onPredatorDetected(Ped predator)
{
	float predatorDistanceToPlayer = distance(predator, player);
	float predatorDistanceToCompanion = distance(state->companionDog, predator);

	if (isPedASnake(predator))
	{
		if (predatorDistanceToCompanion > DataFiles::DogMeta->getInt("snakes_detection_radius"))
		{
			return;
		}

		float snakesAttackDistance = DataFiles::DogMeta->getInt("attack_snakes_distace");
		if (predatorDistanceToCompanion < snakesAttackDistance ||
			predatorDistanceToPlayer < snakesAttackDistance)
		{
			state->companionApi->combat(predator);
		}
		else if (!state->companionApi->isAgitated())
		{
			state->currentTask = state->companionApi->warnSnake(predator, 6000);
		}
	}
	else
	{
		int attackDistance = DataFiles::DogMeta->getInt("attack_predators_distance");
		if (DataFiles::Dog->getBool("will_attack_predators") && 
			predatorDistanceToPlayer <= attackDistance && 
			!state->companionApi->isInCombat())
		{
			state->companionApi->combat(predator);
		}
		else if (!state->companionApi->isAgitated())
		{
			state->currentTask = state->companionApi->warn(predator, 10000);
		}
	}

	tutorial("predator_detection");
}

void CompanionEngine::onCompnaionDied()
{
	showHelpText(DataFiles::Lang->get("tutorial.companion_died"));
	log("companion died");
	clearCompanion();
}

void CompanionEngine::dismissCompanion()
{
	log("companion dismissed");
	state->companionApi->dismiss();
	TASK::TASK_ANIMAL_INTERACTION(player, state->companionDog, MISC::GET_HASH_KEY("Interaction_Dog_Patting"), 0, 0);
	PED::REMOVE_PED_FROM_GROUP(state->companionDog);
	PED::SET_PED_CAN_BE_TARGETTED_BY_PLAYER(state->companionDog, PLAYER::PLAYER_ID(), true);
	PED::SET_PED_RELATIONSHIP_GROUP_HASH(state->companionDog, PED::GET_PED_RELATIONSHIP_GROUP_DEFAULT_HASH(state->companionDog));
	ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&state->companionDog);
	clearCompanion();
}

void CompanionEngine::clearCompanion()
{
	DECORATOR::DECOR_SET_INT(state->companionDog, "SH_CMP_companion", 0);
	state->companionDog = NULL;
	deleteBlipSafe(&state->companionBlip);
	DataFiles::Dog->set("model", "");
	DataFiles::Dog->set("name", "");
	DataFiles::Dog->set("bonding_level", "");
	DataFiles::Dog->set("max_health", "");
	DataFiles::Dog->save();

	state->dismissPrompt->hide();
	state->feedPrompt->hide();
	state->praisePrompt->hide();
	state->stayPrompt->hide();
	state->renamePrompt->hide();
	state->followPrompt->hide();

	if (state->attackPrompt)
	{
		state->attackPrompt->hide();
	}

	log("companion has been cleared");
}

void CompanionEngine::renameCompanion(const char* name)
{
	if (name && strlen(name))
	{
		log(string("companion renamed to: ").append(name).c_str());
		DataFiles::Dog->set("name", name);
		state->companionApi->setName(name);
		state->companionApi->getPraised(player);
		setBlipLabel(state->companionBlip, name);
		DataFiles::Dog->save();
	}
}

void CompanionEngine::tutorial(const char* tutorialKey)
{
	if (!DataFiles::TutorialFlags->getBool(tutorialKey))
	{
		showHelpText(DataFiles::Lang->get(string("tutorial.").append(tutorialKey).c_str()), 8000);
		log(string("tutorial seen: ").append(tutorialKey));
		DataFiles::TutorialFlags->set(tutorialKey, 1);
		DataFiles::TutorialFlags->save();
	}
}