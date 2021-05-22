#include "Main.h"

float CoreX = 0.139;
float CoreY = 0.927;
float CoreOutlineWidth = 0.0242;
float CoreOutlineHeight = CoreOutlineWidth * 16 / 9;

float CoreBackgroundWidth = CoreOutlineWidth * 0.9;
float CoreBackgroundHeight = CoreOutlineHeight * 0.9;
float CoreBackgroundX = CoreX + (CoreOutlineWidth - CoreBackgroundWidth) / 2;
float CoreBackgroundY = CoreY + (CoreOutlineHeight - CoreBackgroundHeight) / 2;

float CoreMeterWidth = CoreOutlineWidth * 1.05;
float CoreMeterHeight = CoreOutlineHeight * 1.05;
float CoreMeterX = CoreX + (CoreOutlineWidth- CoreMeterWidth) / 2;
float CoreMeterY = CoreY + (CoreOutlineHeight -CoreMeterHeight) / 2;

float CoreStateWidth = CoreOutlineWidth * 0.9;
float CoreStateHeight = CoreOutlineHeight * 0.9;
float CoreStateX = CoreX + (CoreOutlineWidth - CoreStateWidth) / 2;
float CoreStateY = CoreY + (CoreOutlineHeight - CoreStateHeight) / 2;

float CoreModifierWidth = CoreOutlineWidth * 1.1;
float CoreModifierHeight = CoreModifierWidth * 16 / 9;
float CoreModifierX = CoreX + (CoreOutlineWidth - CoreModifierWidth) / 2;
float CoreModifierY = CoreY + (CoreOutlineHeight - CoreModifierHeight) / 2;

float CoreIconWidth = CoreOutlineWidth * 0.5;
float CoreIconHeight = CoreIconWidth * 16 / 9;
float CoreIconX = CoreX + (CoreOutlineWidth - CoreIconWidth) / 2;
float CoreIconY = CoreMeterY + CoreMeterHeight - CoreIconHeight * 3 / 4;

RGB AgitationModifierColor = { 255, 0, 0 };
const char* HealthCoreTextureDict = "rpg_core_health";

ComapnionCoresUI::ComapnionCoresUI(Ped ped, int bondingLevel)
{
	this->companion = ped;
	this->bondingLevel = bondingLevel;
	this->isOpen = false;
	this->modifier = CompanionCoreModifiers::None;
}

void ComapnionCoresUI::setPed(Ped ped)
{
	this->companion = ped;
}

void ComapnionCoresUI::setBondingLevel(int bondingLevel)
{
	this->bondingLevel = bondingLevel;
}

void ComapnionCoresUI::setCoreModifier(CompanionCoreModifiers modifier)
{
	this->modifier = modifier;
}

void ComapnionCoresUI::update()
{
	if (isOpen)
	{
		draw();
	}
}

void ComapnionCoresUI::show()
{
	isOpen = true;
}

void ComapnionCoresUI::hide() 
{
	isOpen = false;
}

void ComapnionCoresUI::draw()
{
	float healthRate = (float)ENTITY::GET_ENTITY_HEALTH(companion) / ENTITY::GET_ENTITY_MAX_HEALTH(companion, 1);
	switch (modifier)
	{
	case CompanionCoreModifiers::Agitated:
		drawCore(healthRate, HealthCoreTextureDict, 1, "rpg_agitation", &AgitationModifierColor);
		break;
	default:
		drawCore(healthRate, HealthCoreTextureDict, 1);
	}
}

void ComapnionCoresUI::drawCore(float meterFill, const char* coreTypeTextureDict, float coreFill, const char* modifier, RGB* modifierColor)
{
	int meterIndex = (int)(meterFill * 100 - 1);
	if (meterIndex < 0 || meterIndex > 99)
	{
		return;
	}

	drawSprite("rpg_textures", "rpg_background", CoreBackgroundX, CoreBackgroundY, CoreBackgroundWidth, CoreBackgroundHeight, 0, 0, 0, 0, 255);
	drawSprite("rpg_meter_track", "rpg_meter_track_9", CoreX, CoreY, CoreOutlineWidth, CoreOutlineHeight, 0, 109, 109, 109, 255);
	drawSprite("rpg_meter", (char*)string("rpg_meter_").append(to_string(meterIndex)).c_str(), CoreMeterX, CoreMeterY, CoreMeterWidth, CoreMeterHeight, 0, 229, 229, 229, 255);

	if (modifier)
	{
		if (!modifierColor)
		{
			if (coreTypeTextureDict)
			{
				modifierColor = new RGB{ 0, 0, 0 };
			}
			else
			{
				modifierColor = new RGB{ 229, 229, 299 };
			}
		}

		drawSprite("rpg_textures", (char*)modifier, CoreModifierX, CoreModifierY, CoreModifierWidth, CoreModifierHeight, 0, modifierColor->r, modifierColor->g, modifierColor->b, 255);
	}
	else if (coreTypeTextureDict)
	{
		int coreStateIndex = (int)(coreFill * 16 - 1);
		drawSprite((char*)coreTypeTextureDict, (char*)string("core_state_").append(to_string(coreStateIndex)).c_str(), CoreStateX, CoreStateY, CoreStateWidth, CoreStateHeight, 0, 229, 229, 229, 255);
	}

	drawSprite("blips", "blip_animal", CoreIconX, CoreIconY, CoreIconWidth, CoreIconHeight, 0, 229, 229, 229, 255);
}