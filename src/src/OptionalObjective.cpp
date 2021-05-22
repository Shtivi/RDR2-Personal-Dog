#include "Main.h"
#include <random>

map<OptionalObjectives, OptionalObjective*> optionalObjectivesCache =
{
	{ OptionalObjectives::Stealth, new OptionalObjective(OptionalObjectives::Stealth, 10, "Assassin", "menu_textures", "menu_icon_spectate_blocked", "Assassinate your target with no\nwitnesses") },
	{ OptionalObjectives::LongDistanceKill, new OptionalObjective(OptionalObjectives::LongDistanceKill, 6, "American Sniper", "BLIPS", "blip_region_hunting", "Kill your target from afar using a\nsniper rifle") },
	{ OptionalObjectives::Headshot, new OptionalObjective(OptionalObjectives::Headshot, 5, "Headshot", "toast_challenges_sharpshooter", "challenge_sharpshooter_7", "Shoot your target in the head") },
	{ OptionalObjectives::MeleeKill, new OptionalObjective(OptionalObjectives::MeleeKill, 8, "Close and Personal", "INVENTORY_ITEMS", "weapon_melee_knife", "Kill your target with a melee weapon") },
	{ OptionalObjectives::Dynamite, new OptionalObjective(OptionalObjectives::Dynamite, 6, "Dynamite", "INVENTORY_ITEMS", "ammo_dynamite_normal", "Kill your target using a dynamite") },
	{ OptionalObjectives::TieYourLasso, new OptionalObjective(OptionalObjectives::TieYourLasso, 10, "Hangman", "INVENTORY_ITEMS", "weapon_lasso", "Use TieYourLasso mod and hang your\ntarget") },
	{ OptionalObjectives::BurnEmAll, new OptionalObjective(OptionalObjectives::BurnEmAll, 8, "Burn 'Em All", "satchel_textures", "satchel_nav_fire", "Set your target on fire") },
	{ OptionalObjectives::TrainTracks, new OptionalObjective(OptionalObjectives::TrainTracks, 15, "Train Food", "toast_challenges_bandit", "challenge_bandit_9", "Tie your targets to train tracks") },
};

OptionalObjective* OptionalObjective::fromID(OptionalObjectives id)
{
	return optionalObjectivesCache[id];
}

vector<OptionalObjective*>* OptionalObjective::list()
{
	vector<OptionalObjective*>* results = new vector<OptionalObjective*>();;

	for (pair<OptionalObjectives, OptionalObjective*> entry : optionalObjectivesCache)
	{
		results->push_back(entry.second);
	}

	return results;
}

vector<OptionalObjectives>* OptionalObjective::listIds()
{
	vector<OptionalObjectives>* results = new vector<OptionalObjectives>();

	for (pair<OptionalObjectives, OptionalObjective*> entry : optionalObjectivesCache)
	{
		results->push_back(entry.first);
	}

	return results;
}

vector<OptionalObjectives>* OptionalObjective::random(int count)
{
	vector<OptionalObjectives>* all = listIds();
	vector<OptionalObjectives>* results = new vector<OptionalObjectives>();

	while (count)
	{
		int nextIndex = rndInt(0, all->size());
		vector<OptionalObjectives>::iterator nextPtr = all->begin() + nextIndex;
		results->push_back(*nextPtr);
		all->erase(nextPtr);
		count--;
	}

	return results;
}

OptionalObjective::OptionalObjective(OptionalObjectives id, int reward, const char* displayName, const char* textureDict, const char* textureName, const char* description)
{
	this->id = id;
	this->reward = reward;
	this->displayName = displayName;
	this->textureDictionary = textureDict;
	this->textureName = textureName;
	this->description = description;
}