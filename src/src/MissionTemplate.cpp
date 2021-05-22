#include "Main.h"

static map<MissionTemplateType, MissionTemplate*> templates = {
	{ MissionTemplateType::TransportedPrisoner, new MissionTemplate(MissionTemplateType::TransportedPrisoner, 75, HonorEffect::None, "Prisoner", "Track the prison wagon down and\nkill the Prisoner.") },
	{ MissionTemplateType::BusinessManConvoy, new MissionTemplate(MissionTemplateType::BusinessManConvoy, 50, HonorEffect::None, "Business Man", "Track the Business man's convoy down\nand assassinate him.") },
	{ MissionTemplateType::TrainStation, new MissionTemplate(MissionTemplateType::TrainStation, 40, HonorEffect::Negative, "Citizen", "Get to the train station and\nassassinate the Target.") },
	{ MissionTemplateType::Lawman, new MissionTemplate(MissionTemplateType::Lawman, 90, HonorEffect::None, "Lawman", "Find and assassinate the Lawman.") },
	{ MissionTemplateType::Saloon, new MissionTemplate(MissionTemplateType::Saloon, 40, HonorEffect::Negative, "Citizen", "Go to the saloon and kill the Target.") },
	{ MissionTemplateType::JailPrisoner, new MissionTemplate(MissionTemplateType::JailPrisoner, 80, HonorEffect::None, "Jail Prisoner", "Go to the town's jail and\nassassinate the Prisoner.") },
	{ MissionTemplateType::BanditsCamp, new MissionTemplate(MissionTemplateType::BanditsCamp, 50, HonorEffect::None, "Bandit", "Locate and eliminate the Bandit.\nHe is camping somewhere around town.") },
	{ MissionTemplateType::Riders, new MissionTemplate(MissionTemplateType::Riders, 50, HonorEffect::None, "Bandit", "Track down and eliminate the Bandit.\nHe is travelling somewhere nearby.") },
	{ MissionTemplateType::Hunter, new MissionTemplate(MissionTemplateType::Hunter, 45, HonorEffect::Negative, "Hunter", "Track down and eliminate the Hunter.") },
	{ MissionTemplateType::AmbientPed, new MissionTemplate(MissionTemplateType::AmbientPed, 40, HonorEffect::None, "Citizen", "Track down and eliminate the Target\naround it's last known location.") },
	{ MissionTemplateType::AnnesburgMineManager, new MissionTemplate(MissionTemplateType::AnnesburgMineManager, 75, HonorEffect::Negative, "Mine Manager", "Find and assassinate Annesburg's mine manager.") },
	{ MissionTemplateType::AnnesburgMineGuard, new MissionTemplate(MissionTemplateType::AnnesburgMineGuard, 55, HonorEffect::None, "Mine Guard", "Find and assassinate the Mine Guard.") },
	{ MissionTemplateType::BountyHuntersRiders, new MissionTemplate(MissionTemplateType::BountyHuntersRiders, 60, HonorEffect::None, "Bounty Hunter", "Find and assassinate the\nBounty hunter.") },
	{ MissionTemplateType::BanditsShack, new MissionTemplate(MissionTemplateType::BanditsShack, 50, HonorEffect::None, "Bandit", "Locate and eliminate the Bandit.\nHe is camping somewhere around town.") },
	{ MissionTemplateType::BanditHideout, new MissionTemplate(MissionTemplateType::BanditHideout, 65, HonorEffect::None, "Bandit", "Locate and eliminate the Bandit.\nHe is camping with his gang.") },
	{ MissionTemplateType::AmbientRancher, new MissionTemplate(MissionTemplateType::AmbientRancher, 40, HonorEffect::Negative, "Rancher", "Locate and kill the Rancher.") },
	{ MissionTemplateType::ArmyConvoy, new MissionTemplate(MissionTemplateType::ArmyConvoy, 70, HonorEffect::None, "Soldier", "Locate and kill the Soldier.") },
	{ MissionTemplateType::MoonshinerConvoy, new MissionTemplate(MissionTemplateType::MoonshinerConvoy, 50, HonorEffect::None, "Moonshiner", "Find the Moonshiner's wagon\nand kill him.") },
	{ MissionTemplateType::OilTraderConvoy, new MissionTemplate(MissionTemplateType::OilTraderConvoy, 45, HonorEffect::Negative, "Oil Traider", "Find the Oil Trader's convoy\nand kill him.") },
	{ MissionTemplateType::Grey, new MissionTemplate(MissionTemplateType::Grey, 50, HonorEffect::None, "Grey", "Find the Grey and eliminate him.\nHe is somewhere around Caliga Hall.") },
	{ MissionTemplateType::Braithwaite, new MissionTemplate(MissionTemplateType::Braithwaite, 50, HonorEffect::None, "Braithwaite", "Find the Braithwaite and\nand kill him. He is somewhere\naround the Braithwaite Manor.") },
	{ MissionTemplateType::TrainTraveler, new MissionTemplate(MissionTemplateType::TrainTraveler, 60, HonorEffect::Negative, "Train Traveler", "Locate the train and assassinate\nthe target.") },
	{ MissionTemplateType::TrainWorker, new MissionTemplate(MissionTemplateType::TrainWorker, 40, HonorEffect::Negative, "Train Worker", "Find and assassinate the train worker.") },
	{ MissionTemplateType::HotelRoom, new MissionTemplate(MissionTemplateType::HotelRoom, 40, HonorEffect::Negative, "Business Man", "Find and assassinate the business man.\nHe is staying in the hotel.") }
};

MissionTemplate* MissionTemplate::fromType(MissionTemplateType type)
{
	return templates[type];
}

MissionTemplate::MissionTemplate(MissionTemplateType type, int baseReward, HonorEffect honorEffect, const char* title, const char* description)
{
	this->type = type;
	this->baseReward = baseReward;
	this->title = title;
	this->description = description;
	this->honorEffect = honorEffect;
}

const char* MissionTemplate::baseRewardDisplay()
{
	stringstream str;
	str << '$' << baseReward;
	return str.str().c_str();
}