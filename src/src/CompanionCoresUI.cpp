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
RGB LowHealthFillColor = { 255, 0, 0 };
RGB Black = { 0,0,0};
RGB DefaultBrightColor = { 229, 229, 229 };

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
	float healthCoreValue = (float)DECORATOR::DECOR_GET_INT(companion, "SH_CMP_health_core") / 100;
	RGB* fillColor = healthCoreValue < 0.3 ? &LowHealthFillColor : &DefaultBrightColor;
	switch (modifier)
	{
	case CompanionCoreModifiers::Agitated:
		drawCore(healthRate, HealthCoreTextureDict, healthCoreValue, fillColor, "rpg_agitation", &AgitationModifierColor);
		break;
	default:
		drawCore(healthRate, HealthCoreTextureDict, healthCoreValue, fillColor);
	}
}

void ComapnionCoresUI::drawCore(float meterFill, const char* coreTypeTextureDict, float coreFill, RGB* fillColor, const char* modifier, RGB* modifierColor)
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
				modifierColor = &Black;
			}
			else
			{
				modifierColor = &DefaultBrightColor;
			}
		}

		drawSprite("rpg_textures", (char*)modifier, CoreModifierX, CoreModifierY, CoreModifierWidth, CoreModifierHeight, 0, modifierColor->r, modifierColor->g, modifierColor->b, 255);
	}
	else if (coreTypeTextureDict)
	{
		int coreStateIndex = max(0, (int)(coreFill * 16 - 1));
		if (!fillColor)
		{
			fillColor = &DefaultBrightColor;
		}
		drawSprite((char*)coreTypeTextureDict, (char*)string("core_state_").append(to_string(coreStateIndex)).c_str(), CoreStateX, CoreStateY, CoreStateWidth, CoreStateHeight, 0, fillColor->r, fillColor->g, fillColor->b, 255);
	}

	drawSprite("blips", "blip_animal", CoreIconX, CoreIconY, CoreIconWidth, CoreIconHeight, 0, 229, 229, 229, 255);
}