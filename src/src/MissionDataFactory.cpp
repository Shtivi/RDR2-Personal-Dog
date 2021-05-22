#include "Main.h"

MissionDataFactory::MissionDataFactory()
{
}

MissionData* MissionDataFactory::generateMission(int id, MapArea* area)
{
	vector<MissionTemplateType>* templates = area->getMissionTemplates();
	MissionTemplateType missionTemplate = templates->at(rndInt(0, templates->size()));
	return generateMission(id, area, missionTemplate);
}

MissionData* MissionDataFactory::generateMission(int id, MapArea* area, MissionTemplateType missionTemplate)
{
	MissionData* missionData = NULL;

	log(string("generating mission data. template type: ").append(to_string((int)missionTemplate)).append(", area: ").append(area->getDisplayName()));

	switch (missionTemplate)
	{
	case MissionTemplateType::TransportedPrisoner:
	{
		missionData = generateTransportedPrisonerMission(id, area);
		break;
	}
	case MissionTemplateType::TrainStation:
	{
		missionData = generateTrainStationMission(id, area);
		break;
	}
	case MissionTemplateType::BusinessManConvoy:
	{
		missionData = generateBusinessManConvoyMission(id, area);
		break;
	}
	case MissionTemplateType::JailPrisoner:
	{
		missionData = generateJailPrisonerMission(id, area);
		break;
	}
	case MissionTemplateType::Saloon:
	{
		missionData = generateSaloonMission(id, area);
		break;
	}
	case MissionTemplateType::Lawman:
	{
		missionData = generateLawmanMission(id, area);
		break;
	}
	case MissionTemplateType::BanditsCamp:
	{
		missionData = genereateBanditsCampMission(id, area);
		break;
	}
	case MissionTemplateType::Riders:
	{
		missionData = generateRidersMission(id, area);
		break;
	}
	case MissionTemplateType::Hunter:
	{
		missionData = generateHunterMission(id, area);
		break;
	}
	case MissionTemplateType::AmbientPed:
	{
		missionData = generateAmbientPedMission(id, area);
		break;
	}
	case MissionTemplateType::AnnesburgMineManager:
	{
		missionData = generateAnnesburgMineManagerMission(id, area);
		break;
	}
	case MissionTemplateType::AnnesburgMineGuard:
	{
		missionData = generateAnnesburgMineGuardMission(id, area);
		break;
	}
	case MissionTemplateType::BanditsShack:
	{
		missionData = generateBanditsShackMission(id, area);
		break;
	}
	case MissionTemplateType::BanditHideout:
	{
		missionData = generateBanditHideoutMission(id, area);
		break;
	}
	case MissionTemplateType::AmbientRancher:
	{
		missionData = generateAmbientRancherMission(id, area);
		break;
	}
	case MissionTemplateType::ArmyConvoy:
	{
		missionData = generateArmyConvoyMission(id, area);
		break;
	}
	case MissionTemplateType::MoonshinerConvoy:
	{
		missionData = generateMoonshinerConvoyMission(id, area);
		break;
	}
	case MissionTemplateType::OilTraderConvoy:
	{
		missionData = generateOilTraderConvoyMission(id, area);
		break;
	}
	case MissionTemplateType::BountyHuntersRiders:
	{
		missionData = generateBountyHuntersRidersMission(id, area);
		break;
	}
	case MissionTemplateType::Grey:
	{
		missionData = generateGreyMission(id, area);
		break;
	}
	case MissionTemplateType::TrainTraveler:
	{
		missionData = generateTrainTravelerMission(id, area);
		break;
	}
	case MissionTemplateType::TrainWorker:
	{
		missionData = generateTrainWorkerMission(id, area);
		break;
	}
	case MissionTemplateType::HotelRoom:
	{
		missionData = generateHotelRoomMission(id, area);
		break;
	}
	default:
		log(string("MissionDataFactory: could not generate a mission of generated template: ")
			.append(to_string((int)missionTemplate))
			.append(" at area: ")
			.append(area->getDisplayName()));
		return NULL;
	}

	if (!missionData->optionalObjectives)
	{
		log("generating random challenges");
		missionData->optionalObjectives = OptionalObjective::random(3);
	}

	return missionData;
}

MissionData* MissionDataFactory::generateTransportedPrisonerMission(int id, MapArea* area)
{
	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = area->getRandomWildernessLocation();
	data->areaRadius = 110;
	data->identificationDistance = 35;
	data->difficultyLevel = (MissionDifficulty)rndInt(1, 3);
	data->templateType = MissionTemplateType::TransportedPrisoner;
	

	return data;
}

MissionData* MissionDataFactory::generateTrainStationMission(int id, MapArea* area)
{
	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = area->getRandomPostOfficeLocation();
	data->areaRadius = 55;
	data->identificationDistance = 10;
	data->difficultyLevel = MissionDifficulty::Normal;
	data->templateType = MissionTemplateType::TrainStation;
	

	return data;
}

MissionData* MissionDataFactory::generateBusinessManConvoyMission(int id, MapArea* area)
{
	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = area->getRandomWildernessLocation();
	data->areaRadius = 110;
	data->identificationDistance = 35;
	data->difficultyLevel = (MissionDifficulty)rndInt(1, 4);
	data->templateType = MissionTemplateType::BusinessManConvoy;
	

	return data;
}

MissionData* MissionDataFactory::generateLawmanMission(int id, MapArea* area)
{
	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = area->randomLocationInTown();
	data->areaRadius = 40;
	data->identificationDistance = 12;
	data->difficultyLevel = MissionDifficulty::Normal;
	data->templateType = MissionTemplateType::Lawman;
	

	return data;
}

MissionData* MissionDataFactory::generateSaloonMission(int id, MapArea* area)
{
	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = area->getRandomSaloonLocation();
	data->areaRadius = 35;
	data->identificationDistance = 8;
	data->difficultyLevel = MissionDifficulty::Normal;
	data->templateType = MissionTemplateType::Saloon;
	

	return data;
}

MissionData* MissionDataFactory::generateJailPrisonerMission(int id, MapArea* area)
{
	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = area->getRandomJailCellLocation().first;
	data->areaRadius = 20;
	data->identificationDistance = 5;
	data->difficultyLevel = MissionDifficulty::Normal;
	data->templateType = MissionTemplateType::JailPrisoner;
	

	return data;
}

MissionData* MissionDataFactory::genereateBanditsCampMission(int id, MapArea* area)
{
	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = area->getRandomCampingLocation();
	data->areaRadius = 120;
	data->identificationDistance = 30;
	data->difficultyLevel = (MissionDifficulty) rndInt(1, 3);
	data->templateType = MissionTemplateType::BanditsCamp;
	

	return data;
}

MissionData* MissionDataFactory::generateRidersMission(int id, MapArea* area)
{
	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = area->getRandomRiderRoadLocation();
	data->areaRadius = 120;
	data->identificationDistance = 30;
	data->difficultyLevel = (MissionDifficulty)rndInt(1, 3);
	data->templateType = MissionTemplateType::Riders;
	

	return data;
}

MissionData* MissionDataFactory::generateHunterMission(int id, MapArea* area)
{
	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = area->getRandomCampingLocation();
	data->areaRadius = 100;
	data->identificationDistance = 20;
	data->difficultyLevel = MissionDifficulty::Normal;
	data->templateType = MissionTemplateType::Hunter;
	

	return data;
}

MissionData* MissionDataFactory::generateAmbientPedMission(int id, MapArea* area)
{
	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = area->randomLocationInTown();
	data->areaRadius = 60;
	data->identificationDistance = 20;
	data->difficultyLevel = MissionDifficulty::Normal;
	data->templateType = MissionTemplateType::AmbientPed;
	

	return data;
}

MissionData* MissionDataFactory::generateAnnesburgMineManagerMission(int id, MapArea* area)
{
	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = toVector3(2920.47, 1379.14, 56.2244);
	data->areaRadius = 70;
	data->identificationDistance = 20;
	data->difficultyLevel = MissionDifficulty::Normal;
	data->templateType = MissionTemplateType::AnnesburgMineManager;
	

	return data;
}

MissionData* MissionDataFactory::generateAnnesburgMineGuardMission(int id, MapArea* area)
{
	vector<Vector3> possibleLocations = {
		toVector3(2947.32, 1378.41, 51.323),
		toVector3(2960.08, 1378.9, 51.2245),
		toVector3(2789.74, 1339.43, 71.3096),
		toVector3(2870.36, 1408.77, 67.3794),
		toVector3(2853.88, 1447.85, 69.2677)
	};

	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = possibleLocations.at(rndInt(0, possibleLocations.size()));
	data->areaRadius = 45;
	data->identificationDistance = 20;
	data->difficultyLevel = MissionDifficulty::Normal;
	data->templateType = MissionTemplateType::AnnesburgMineGuard;
	

	return data;
}

MissionData* MissionDataFactory::generateBountyHuntersRidersMission(int id, MapArea* area)
{
	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = area->getRandomRiderRoadLocation();
	data->areaRadius = 120;
	data->identificationDistance = 30;
	data->difficultyLevel = (MissionDifficulty)rndInt(1, 4);
	data->templateType = MissionTemplateType::BountyHuntersRiders;
	

	return data;
}

MissionData* MissionDataFactory::generateBanditsShackMission(int id, MapArea* area)
{
	Place* shack = area->getRandomPlaceByType(PlaceType::Shack);

	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = shack->getCoords();
	data->areaRadius = shack->getRadius() * 2;
	data->identificationDistance = 10;
	data->difficultyLevel = (MissionDifficulty)rndInt(1, 3);
	data->templateType = MissionTemplateType::BanditsShack;
	

	return data;
}

MissionData* MissionDataFactory::generateBanditHideoutMission(int id, MapArea* area)
{
	Place* hideout = area->getRandomPlaceByType(PlaceType::GangHideout);

	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = hideout->getCoords();
	data->areaRadius = hideout->getRadius() * 1.5;
	data->identificationDistance = 30;
	data->difficultyLevel = MissionDifficulty::Normal;
	data->templateType = MissionTemplateType::BanditHideout;
	

	return data;
}

MissionData* MissionDataFactory::generateAmbientRancherMission(int id, MapArea* area)
{
	Place* ranch = area->getRandomPlaceByType(PlaceType::Ranch);

	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = ranch->getCoords();
	data->areaRadius = ranch->getRadius() * 1.5;
	data->identificationDistance = 20;
	data->difficultyLevel = MissionDifficulty::Normal;
	data->templateType = MissionTemplateType::AmbientRancher;
	

	return data;
}

MissionData* MissionDataFactory::generateArmyConvoyMission(int id, MapArea* area)
{
	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = area->getRandomWildernessLocation();
	data->areaRadius = 110;
	data->identificationDistance = 30;
	data->difficultyLevel = (MissionDifficulty)rndInt(1, 4);
	data->templateType = MissionTemplateType::ArmyConvoy;
	

	return data;
}

MissionData* MissionDataFactory::generateMoonshinerConvoyMission(int id, MapArea* area)
{
	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = area->getRandomWildernessLocation();
	data->areaRadius = 110;
	data->identificationDistance = 30;
	data->difficultyLevel = (MissionDifficulty)rndInt(1, 4);
	data->templateType = MissionTemplateType::MoonshinerConvoy;
	

	return data;
}

MissionData* MissionDataFactory::generateOilTraderConvoyMission(int id, MapArea* area)
{
	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = area->getRandomWildernessLocation();
	data->areaRadius = 110;
	data->identificationDistance = 35;
	data->difficultyLevel = (MissionDifficulty)rndInt(1, 4);
	data->templateType = MissionTemplateType::OilTraderConvoy;
	

	return data;
}

MissionData* MissionDataFactory::generateGreyMission(int id, MapArea* area)
{
	Place* caligaHall = area->getRandomPlaceByType(PlaceType::CaligaHall);

	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = caligaHall->getCoords();
	data->areaRadius = caligaHall->getRadius();
	data->identificationDistance = 20;
	data->difficultyLevel = (MissionDifficulty)rndInt(1, 3);
	data->templateType = MissionTemplateType::Grey;
	

	return data;
}

MissionData* MissionDataFactory::generateBraithwaiteMission(int id, MapArea* area)
{
	Place* braithwaiteManor = area->getRandomPlaceByType(PlaceType::BraithwaiteManor);

	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = braithwaiteManor->getCoords();
	data->areaRadius = braithwaiteManor->getRadius();
	data->identificationDistance = 30;
	data->difficultyLevel = (MissionDifficulty)rndInt(1, 3);
	data->templateType = MissionTemplateType::Braithwaite;
	

	return data;
}

MissionData* MissionDataFactory::generateTrainTravelerMission(int id, MapArea* area)
{
	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = area->getRandomTrainTrackLocation().first;
	data->areaRadius = 80;
	data->identificationDistance = 10;
	data->difficultyLevel = (MissionDifficulty)rndInt(1, 3);
	data->templateType = MissionTemplateType::TrainTraveler;
	

	return data;
}

MissionData* MissionDataFactory::generateTrainWorkerMission(int id, MapArea* area)
{
	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = area->getRandomTrainTrackLocation().first;
	data->areaRadius = 110;
	data->identificationDistance = 15;
	data->difficultyLevel = (MissionDifficulty)rndInt(1, 3);
	data->templateType = MissionTemplateType::TrainWorker;
	

	return data;
}

MissionData* MissionDataFactory::generateHotelRoomMission(int id, MapArea* area)
{
	Place* room = area->getRandomPlaceByType(PlaceType::HotelRoom);

	MissionData* data = new MissionData(area);
	data->id = id;
	data->area = area->getAreaId();
	data->startPosition = room->getCoords();
	data->areaRadius = room->getRadius() * 1.5;
	data->identificationDistance = 8;
	data->difficultyLevel = MissionDifficulty::Normal;
	data->templateType = MissionTemplateType::HotelRoom;


	return data;
}