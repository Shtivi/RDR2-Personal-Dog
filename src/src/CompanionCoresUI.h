#pragma once

enum class CompanionCoreModifiers : int
{
	None,
	Agitated
};

class ComapnionCoresUI
{
private:
	Ped companion;
	int bondingLevel;
	CompanionCoreModifiers modifier;
	bool isOpen;

public:
	ComapnionCoresUI(Ped ped = NULL, int bondingLevel = 3);
	void setPed(Ped ped);
	void setBondingLevel(int bondingLevel);
	void setCoreModifier(CompanionCoreModifiers modifier);
	void update();
	void show();
	void hide();

private:
	void draw();
	void drawCore(float meterFill, const char* coreTypeTextureDict = NULL, float coreFill = 1, const char* modifier = NULL, RGB* modifierColor = NULL);
};