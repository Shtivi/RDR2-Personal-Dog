#pragma once

class CompanionCommands
{
public:
	static void triggerFollow(RootState* state);
	static void commandFollow(RootState* state);
	static void commandStay(RootState* state);
	static void commandPraise(RootState* state);
	static void commandRetrieve(RootState* state);
	static void commandAttack(RootState* state, Ped target);
	static void commandTrack(RootState* state, Ped target);
};