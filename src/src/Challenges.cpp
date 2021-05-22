#include "Main.h"

Challenges::Challenges()
{

}

void Challenges::setStatus(OptionalObjectives challengeType, bool completed)
{
	challenges.insert(challengeType);
	challengeStatuses[challengeType] = completed;
}

bool Challenges::getStatus(OptionalObjectives challengeType)
{
	if (challengeStatuses.find(challengeType) == challengeStatuses.end())
	{
		return false;
	}

	return challengeStatuses[challengeType];
}

void Challenges::complete(OptionalObjectives challengeType)
{
	setStatus(challengeType, true);
}

set<OptionalObjectives>* Challenges::list()
{
	return &challenges;
}