#pragma once

class PassangerAI : public PedBehavior
{
private:
	bool isPleading;

public:
	PassangerAI(Ped ped);

	virtual void update();
	void cower(Ped cowerFrom);
	void handsUp(Ped cowerFrom);
	bool hasGunPointedAt();
};