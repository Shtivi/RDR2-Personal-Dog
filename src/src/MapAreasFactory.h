#pragma once

class MapAreasFactory
{
private:
	map<MapAreas, MapArea*> cache;
	PlacesFactory* placesFactory;

public:
	MapAreasFactory(PlacesFactory* placesFactory);
	MapArea* getMapArea(MapAreas mapArea);
	MapArea* getClosestTown(Vector3 location);
	vector<MapArea*> listMapAreas();

private:
	MapArea* createBlackwater();
	MapArea* createRhodes();
	MapArea* createValentine();
	MapArea* createStrawberry();
	MapArea* createTumbleweed();
	MapArea* createArmadillo();
	MapArea* createSaintDenis();
	MapArea* createAnnesburg();
};