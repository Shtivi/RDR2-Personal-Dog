#pragma once

const float HANGING_TRESHOLD = 1.1f;

class AttachedRope : public Rope
{
private:
	int ropeId;
	Entity entity1;
	Entity entity2;
	const char* bone1;
	const char* bone2;
	float ropeLength;
	bool isAttachedToMap;
	bool isEntityHanging;
	bool isWinding;
	bool isUnwinding;

public:
	AttachedRope(Vector3 mapPosition, Entity entity, const char* bone, float length);
	AttachedRope(Entity entity, Vector3 mapPosition, const char* baseObjectName = "p_shotGlass01x");
	AttachedRope(Entity entity, Entity base, const char* bone1, const char* bone2, float length);

	bool getIsAttachedToMap();
	bool getIsEntityHanging();
	bool isExist();
	Entity getAttached();
	Entity getBase();
	
	bool canWind();
	void startWinding();
	void stopWinding();
	bool canUnwind();
	void startUnwinding();
	void stopUnwinding();
	int update();

private:
	Object createMapProp(char* model, Vector3 position);
};