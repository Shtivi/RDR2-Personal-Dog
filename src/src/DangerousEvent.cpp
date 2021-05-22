#include "Main.h"

DangerousEvent::DangerousEvent()
{
	eventType = DangerousEvents::None;
	sourceLocation = toVector3(0, 0, 0);
	sourceEntity = NULL;
	responsible = NULL;
}

DangerousEvent::DangerousEvent(DangerousEvents eventType, Vector3 sourceLocation)
	: DangerousEvent()
{
	this->eventType = eventType;
	this->sourceLocation = sourceLocation;
}

DangerousEvent::DangerousEvent(DangerousEvents eventType, Vector3 sourceLocation, Entity responsible)
	: DangerousEvent(eventType, sourceLocation)
{
	this->responsible = responsible;
}

DangerousEvent::DangerousEvent(DangerousEvents eventType, Entity sourceEntity)
	: DangerousEvent(eventType, entityPos(sourceEntity))
{
	this->sourceEntity = sourceEntity;
}

DangerousEvent::DangerousEvent(DangerousEvents eventType, Entity sourceEntity, Entity responsible)
	: DangerousEvent(eventType, sourceEntity)
{
	this->responsible = sourceEntity;
}

DangerousEvent::DangerousEvent(DangerousEvents eventType, vector<Entity> sourceEntities)
	: DangerousEvent(eventType, sourceEntities[0])
{
	this->sourceEntities = sourceEntities;
}
