#pragma once

enum class CoachRobberyMissionStage : int 
{
	Initializing = 0,
	BuyTip,
	ArriveAtArea,
	WaitForWagon,
	Robbery,
	TakeOver,
	Deliver,
	Done
};

enum class CoachRobberyMissionStatus : int
{
	Unavailable = -1,
	Pending = 0,
	InProgress = 2,
	Completed = 3,
	Failed = 4
};

class CoachRobberyMission
{
public:
	void update();
};