#pragma once

enum class AlertnessStatus : int
{
	Idle = 0,
	Warn = 1,
	Threaten = 2,
	Alerted = 3,
	Combat = 4,
	Flee = 5
};

#include "RiderAI.h"
#include "PassangerAI.h"
#include "RidersGroup.h"