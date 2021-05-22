#pragma once

enum class DangerousEvents : int
{
	None,
	SuspiciousNoise,
	ShotsHeard,
	SuspectDetected,
	DeadBody,
};

class DangerousEvent
{
public:
	DangerousEvents eventType;
	Vector3 sourceLocation;
	Entity sourceEntity;
	Entity responsible;
	vector<Entity> sourceEntities;

	DangerousEvent();
	DangerousEvent(DangerousEvents eventType, Vector3 sourceLocation);
	DangerousEvent(DangerousEvents eventType, Vector3 sourceLocation, Entity responsible);
	DangerousEvent(DangerousEvents eventType, Entity sourceEntity);
	DangerousEvent(DangerousEvents eventType, Entity sourceEntity, Entity responsible);
	DangerousEvent(DangerousEvents eventType, vector<Entity> sourceEntities);
};