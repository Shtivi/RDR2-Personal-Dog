#pragma once
#pragma once

enum VehicleHash
{
	PrivateOpenSleeper02X = 0x7F4258C9,
	PrivateOpenSleeper01X = 0xC27964BE,
	SteamerDummy = 0xAD516118,
	ArmoredCar01X = 0xC8EF11A0,
	ArmoredCar03X = 0x22C976E4,
	PrivateBaggage01X = 0x6A80D253,
	Smuggler02 = 0xCFD1449,
	Keelboat = 0xEF91537F,
	BoatSteam02X = 0x876E6EB7,
	MidlandRefrigeratorCar = 0xECD7E90E,
	MidlandBoxcar05X = 0x39584F5A,
	Caboose01X = 0x8C0224C6,
	Canoe = 0x578D6513,
	CanoeTreeTrunk = 0xD84D4530,
	Cart01 = 0xCEDED274,
	Cart02 = 0x85943FE0,
	Cart03 = 0xAFB2141B,
	Cart04 = 0xDDFBF0AE,
	Cart05 = 0x1656E157,
	Cart06 = 0xD10CECB,
	Cart07 = 0x2D03A4A,
	Cart08 = 0xE98507B4,
	Coach2 = 0x68F6F8F3,
	Coach3 = 0xF7D816B7,
	Coach3Cutscene = 0xF775C720,
	Coach4 = 0x598B238,
	Coach5 = 0x9324CD4E,
	Coach6 = 0xA3EC6EDD,
	Buggy01 = 0xB3C45542,
	Buggy02 = 0xBE696A8C,
	Buggy03 = 0x91068FC7,
	ArmySupplyWagon = 0x276DFE5E,
	ChuckWagon000X = 0x5F27ED25,
	SupplyWagon = 0x9780442F,
	SupplyWagon2 = 0xEC2A1018,
	LogWagon = 0x9A308177,
	LogWagon2 = 0xE89274F1,
	CoalWagon = 0x61EC29C0,
	ChuckWagon002X = 0x69897B5C,
	GatlingGun = 0x1EEBDBE5,
	GatlingMaxim02 = 0x2C3B0296,
	HandCart = 0x590420FE,
	HorseBoat = 0x75BDDBD6,
	HotAirBalloon01 = 0x5EB0BAE0,
	HotchkissCannon = 0x2CA8E7B6,
	MineCart01X = 0xC40B0265,
	NorthFlatCar01X = 0x9A0A187A,
	PrivateFlatCar01X = 0xF228F06,
	NorthPassenger01X = 0x29EA09A9,
	NorthPassenger03X = 0x930442EC,
	PrivatePassenger01X = 0x1C8D173A,
	OilWagon01X = 0xC3E6C004,
	OilWagon02X = 0x824EBBB5,
	Pirogue = 0xF539E5A0,
	Pirogue2 = 0xAE057F07,
	PoliceWagon01X = 0xB203C6B3,
	PoliceWagonGatling01X = 0xB31F8075,
	PrivateCoalCar01X = 0x4717D8D8,
	NorthCoalCar01X = 0xDF86C25A,
	WinterSteamer = 0x6F8F7EE4,
	WinterCoalCar = 0xFD337B7,
	PrivateBoxCar04X = 0x18296CDE,
	PrivateBoxCar02X = 0x4D5B5089,
	PrivateBoxCar01X = 0xC50FC5D0,
	CoalHopper01X = 0x4E018632,
	PrivateObservationCar = 0xF1FE5FB8,
	PrivateArmoured = 0x384E6422,
	PrivateDining01X = 0x22250EF5,
	PrivateRooms01X = 0xEE645446,
	PrivateSteamer01X = 0x55BF98F,
	NorthSteamer01X = 0xF632A662,
	GhostTrainSteamer = 0x9D4E809,
	GhostTrainCoalCar = 0x1C10E9C9,
	GhostTrainPassenger = 0x7724C788,
	GhostTrainCaboose = 0x20925D76,
	RcBoat = 0x5E56769C,
	RowBoat = 0x9FD6BA58,
	RowBoatSwamp = 0xE84E6B74,
	RowBoatSwamp02 = 0xF097BC6C,
	ShipGuama02 = 0xA385E1C7,
	TurbineBoat = 0xE1FE4FD4,
	ShipNbdGuama = 0x427A2D4C,
	ShipNbdGuama2 = 0xC6FA5BFF,
	Skiff = 0xDADC0B67,
	StageCoach001X = 0x7DBBF975,
	StageCoach002X = 0x680D3008,
	StageCoach003X = 0xE7D930EA,
	StageCoach004X_Armoured = 0x9B58946E,
	StageCoach005X = 0xC474677D,
	StageCoach006X = 0xAAFEA8AE,
	Trolley01X = 0xDA152CA6,
	TugBoat2 = 0x7DD49B09,
	Wagon02X = 0xEF1F4829,
	Wagon03X = 0x90C51372,
	Wagon04X = 0x6FBDD4B8,
	Wagon05X = 0x9735A3CF,
	Wagon06X = 0x3C9870A6,
	WagonCircus01X = 0xC2D200FE,
	WagonCircus02X = 0xEE8254F6,
	WagonDoc01X = 0xF7E89A8D,
	WagonPrison01X = 0xCCC649AE,
	WagonWork01X = 0xE96CFEFB,
	WagonDairy01X = 0x8FF91ED,
	WagonTraveller01X = 0xA7FBA623,
	BreachCannon = 0x8979274C,
	Utilliwag = 0x310A4F8B,
	Gatchuck = 0x538529A,
	Gatchuck2 = 0xF53E4390,
};

enum VehicleCargoHash
{
	Luggage1 = -1010941253,
	Luggage2 = 697798539,
	Luggage3 = 1642867136,
	Boxes1 = -1504084621,
	Boxes2 = 1490711830,
	Boxes3 = -1818274469,
	BoxesAndBags = 174891252,
	Barrels = 1291266059,
	ShineBottles = 986026237, // Works well with cart03
	MoonshineBottles = -509228265,
	LoadsOfMoonshine = 1793592017,
	TonsOfMoonshine = -581069256,
	TNT = -1970994897,
	LargeCargo = -1742109836,
	SmallCargo = 1996806757,
	UtillwagCargo = 1306345203,
	UtillwagSmallCargo = -973836254,
	UtillwagExplosives = -1891768214,
	Planks1 = -1021554967,
	Planks2 = 41200892,
	Planks3 = -336173860,
	CampCargo1 = -276407448,
	CampCargo2 = 2121258366,
	LargeCampCargo1 = -1238570474,
	LargeCampCargo2 = -906226543,
	ArmyCargo = 100192451,
	Lanterns = 733112176,
	GeneralGoods = 1848755277,
	HotCoals = -1311425568,
	Posters = -2103461291,
	BloodBottles = -1174840184,
	CottonBails = -323839510
};

enum class TrainConfiguration : Hash 
{
	Passangers = 0xFAB2FFB9,
	LongCargo = 0xF9B038FC,
	PassengersPacificUnion = 0x2D3645FA,
	Handcart = 0x96563327
};

Vehicle createVehicle(char* model, Vector3 pos, float heading = 0);
Vehicle createVehicle(VehicleHash vehicleHash, Vector3 position, float heading = 0);
Vehicle createVehicle(Hash model, Vector3 pos, float heading = 0);
Vehicle createVehicleOnClosestRoad(VehicleHash model, Vector3 aroundLocation, bool flipDirection = false);
Vector3 getClosestRoad(Vector3 aroundLocation, float radius);
void placeVehicleOnClosestRoad(Vehicle vehicle, Vector3 around, bool flipDirection = false);
void setVehicleCargo(Vehicle vehicle, VehicleCargoHash cargoHash);
Ped getPedVehicleOccupant(Ped ped, int seat = -1);
Ped getPedVehicleDriverOrShotgun(Ped ped);
bool isPedDriverOrShotgun(Ped ped);
Vehicle createTrain(TrainConfiguration trainConfigration, Vector3 location, bool direction = true, bool withPassengers = true);
int countTrainCarriages(Vehicle train);
bool isEntityOnTrainTracks(Entity entity);
void setTrainCoords(Entity train, Vector3 coords);
Vector3 getClosestTrainTracks(Vector3 location);
Ped getRandomPedInTrain(Vehicle train, int ignoreCarriage = -1);
int getClosestTrainCarriage(Vehicle train, Vector3 location);
int getClosestTrainCarriage(Vehicle train, Entity entity);