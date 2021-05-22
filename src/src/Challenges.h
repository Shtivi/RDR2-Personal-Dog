#pragma once

class Challenges
{
private:
	set<OptionalObjectives> challenges;
	map<OptionalObjectives, bool> challengeStatuses;

public:
	Challenges();
	void setStatus(OptionalObjectives challengeType, bool completed);
	bool getStatus(OptionalObjectives challengeType);
	void complete(OptionalObjectives challengeType);
	set<OptionalObjectives>* list();
};