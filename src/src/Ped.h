#pragma once

#include "AnimalType.h"

enum HorseSaddleHashes
{
	BIGVALLEY_DOUBLEFORK_SADDLE_000 = 0x8FFCF06B,
	CHARRO_01_IMPROVED_NEW_SADDLE_000 = 0x5546EB7A,
	CHARRO_01_IMPROVED_NEW_SADDLE_001 = 0x8E64DDB5,
	CHARRO_01_IMPROVED_NEW_SADDLE_002 = 0x7092A211,
	CHARRO_01_IMPROVED_NEW_SADDLE_003 = 0xC0C04297,
	CHARRO_01_IMPROVED_NEW_SADDLE_004 = 0xBE703DF7,
	CHARRO_01_IMPROVED_NEW_SADDLE_005 = 0xE5510BB8,
	CHARRO_01_SPECIAL_NEW_SADDLE_000 = 0x7D795D72,
	CHARRO_01_STOCK_NEW_SADDLE_000 = 0x522CCED,
	CHARRO_01_STOCK_NEW_SADDLE_001 = 0x5B45F932,
	CHARRO_01_STOCK_NEW_SADDLE_002 = 0x219D85E2,
	CHARRO_01_STOCK_NEW_SADDLE_003 = 0x7DBB3E1C,
	CHARRO_01_STOCK_NEW_SADDLE_004 = 0x4C1A5ADB,
	CHARRO_01_STOCK_NEW_SADDLE_005 = 0xF1BAA60D,
	HIGHPLAINS_CUTTING_SADDLE_000 = 0xBFD09512,
	MCCLELLAN_01_IMPROVED_NEW_SADDLE_000 = 0x17153A45,
	MCCLELLAN_01_IMPROVED_NEW_SADDLE_001 = 0x5D717C9,
	MCCLELLAN_01_IMPROVED_NEW_SADDLE_002 = 0x4B372288,
	MCCLELLAN_01_IMPROVED_NEW_SADDLE_003 = 0x78F07DFA,
	MCCLELLAN_01_IMPROVED_NEW_SADDLE_004 = 0x2E4668A3,
	MCCLELLAN_01_IMPROVED_NEW_SADDLE_005 = 0x1C14443F,
	MCCLELLAN_01_SPECIAL_NEW_SADDLE_000 = 0x353FC03C,
	MCCLELLAN_01_STOCK_NEW_SADDLE_000 = 0xD97573C1,
	MCCLELLAN_01_STOCK_NEW_SADDLE_001 = 0xF3BEA853,
	MCCLELLAN_01_STOCK_NEW_SADDLE_002 = 0x1F7C4C5,
	MCCLELLAN_01_STOCK_NEW_SADDLE_003 = 0x106961A8,
	MCCLELLAN_01_STOCK_NEW_SADDLE_004 = 0x2ECD9E70,
	MCCLELLAN_01_STOCK_NEW_SADDLE_005 = 0x3D0C3AED,
	MOTHERHUBBARD_01_IMPROVED_NEW_SADDLE_000 = 0x5BBC54C3,
	MOTHERHUBBARD_01_IMPROVED_NEW_SADDLE_001 = 0x8D163776,
	MOTHERHUBBARD_01_IMPROVED_NEW_SADDLE_002 = 0x3E949A74,
	MOTHERHUBBARD_01_IMPROVED_NEW_SADDLE_003 = 0x70BB7EC1,
	MOTHERHUBBARD_01_IMPROVED_NEW_SADDLE_004 = 0xD11CBF82,
	MOTHERHUBBARD_01_IMPROVED_NEW_SADDLE_005 = 0xBA6A921E,
	MOTHERHUBBARD_01_SPECIAL_NEW_SADDLE_000 = 0xD225CCA0,
	MOTHERHUBBARD_01_STOCK_NEW_SADDLE_000 = 0x6D403492,
	MOTHERHUBBARD_01_STOCK_NEW_SADDLE_001 = 0xBB335077,
	MOTHERHUBBARD_01_STOCK_NEW_SADDLE_002 = 0x8D9D754C,
	MOTHERHUBBARD_01_STOCK_NEW_SADDLE_003 = 0x5B6390D9,
	MOTHERHUBBARD_01_STOCK_NEW_SADDLE_004 = 0x14168240,
	MOTHERHUBBARD_01_STOCK_NEW_SADDLE_005 = 0x7FD859C2,
	WESTERN_01_IMPROVED_NEW_SADDLE_000 = 0xC10B5450,
	WESTERN_01_IMPROVED_NEW_SADDLE_001 = 0xD2C8F7CB,
	WESTERN_01_IMPROVED_NEW_SADDLE_002 = 0xD2C8F7CB,
	WESTERN_01_IMPROVED_NEW_SADDLE_003 = 0xF373B920,
	WESTERN_01_IMPROVED_NEW_SADDLE_004 = 0x7A23C686,
	WESTERN_01_IMPROVED_NEW_SADDLE_005 = 0x88C363C5,
	WESTERN_01_SPECIAL_NEW_SADDLE_000 = 0xB5802A5F,
	WESTERN_02_IMPROVED_NEW_SADDLE_000 = 0xB9BE555D,
	WESTERN_02_IMPROVED_NEW_SADDLE_001 = 0x1EC65C0,
	WESTERN_02_IMPROVED_NEW_SADDLE_002 = 0xF2F0045,
	WESTERN_02_IMPROVED_NEW_SADDLE_003 = 0xE52BAC3F,
	WESTERN_02_IMPROVED_NEW_SADDLE_004 = 0xF4B14B4A,
	WESTERN_02_IMPROVED_NEW_SADDLE_005 = 0x3827D232,
	_WESTERN_02_SPECIAL_NEW_SADDLE_000 = 0xDE5A2905,
	WESTERN_03_IMPROVED_NEW_SADDLE_000 = 0x64CEC6DF,
	WESTERN_03_IMPROVED_NEW_SADDLE_001 = 0x9E0C3959,
	WESTERN_03_IMPROVED_NEW_SADDLE_002 = 0x90489DD2,
	WESTERN_03_IMPROVED_NEW_SADDLE_003 = 0xBC52F5E6,
	WESTERN_03_IMPROVED_NEW_SADDLE_004 = 0xD61B2996,
	WESTERN_03_IMPROVED_NEW_SADDLE_005 = 0xC7D58D0B,
	WESTERN_03_SPECIAL_NEW_SADDLE_000 = 0x2BEA8ED4,
	WESTERN_04_IMPROVED_NEW_SADDLE_000 = 0xC454830C,
	WESTERN_04_IMPROVED_NEW_SADDLE_001 = 0xD6BF27E1,
	WESTERN_04_IMPROVED_NEW_SADDLE_002 = 0x24F24446,
	WESTERN_04_IMPROVED_NEW_SADDLE_003 = 0xF4118E4,
	WESTERN_04_IMPROVED_NEW_SADDLE_004 = 0x306806F,
	WESTERN_04_IMPROVED_NEW_SADDLE_005 = 0x70C65BED,
	WESTERN_04_SPECIAL_NEW_SADDLE_000 = 0xC76C46D9
};

Ped createHorse(char* model, Vector3 pos, HorseSaddleHashes saddleType = HorseSaddleHashes::BIGVALLEY_DOUBLEFORK_SADDLE_000);
Ped createAmbientHorse(Vector3 pos);
Ped createPed(char* model, Vector3 pos, float heading = 0);
Ped createPed(const char* model, Vector3 pos);
Ped createPed(int model, Vector3 pos);
Ped createPed(const char* model, Vehicle vehicle, int seatIndex);
Ped createPedOnHorse(char* model, Ped horse, int seatIndex = 1);
bool isPedHogtied(Ped ped);
void playAmbientSpeech(Ped ped, char* speechName, char* voice = NULL);
void giveSaddleToHorse(Ped horse, HorseSaddleHashes saddleHash);
vector<Ped>* getPedGroupMembers(Group groupIndex);
vector<Ped> getNearbyPeds(Ped origin, int limit);
vector<Ped> getNearbyPeds(Ped origin, int limit, float radius, int ignoredPedType = -1, bool filterDead = true);
vector<Ped> getNearbyDeadBodies(Ped origin, int limit, float radius);
bool hasPedClearLosInFront(Ped ped, Entity toEntity, float maxDistance);
Object findUnoccupiedObjectAround(Vector3 location, float maxDistance, int objectHash, float occupationRadius = 2);
Object findUnoccupiedObjectAround(Vector3 location, float maxDistance, vector<int> objectHashes, float occupationRadius = 2);
void playSeatedScenario(Ped ped, Object object, const char* scenarioType, float heading = 180);
int findUnoccupiedScenarioPointAround(Vector3 source, float radius);
bool isPedCop(Ped ped);
bool isPedLawman(Ped ped);
Ped findCarriedPedBy(Ped carrier);
void playAnimation(Ped ped, const char* animName, const char* animDict, int duration = -1, float blendInSpeed = 8.0f, float blendOutSpeed = -8.0f, int flags = 0);
bool isPedADog(Ped ped);
bool isPedASnake(Ped ped);
int getPedSize(Ped ped);

#include "PedBehavior.h"
#include "Conversation.h"