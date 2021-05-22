#include "Main.h"

Prompt::Prompt(const char* text, Hash control)
	: Prompt(text, control, Standard)
{
}

Prompt::Prompt(const char* text, Hash control, PromptMode mode)
{
	handle = UI::_0x04F97DE45A519419(); // _UIPROMPT_REGISTER_BEGIN
	setControl(control);
	setText(text);
	setTargetEntity(NULL);
	setMode(mode);
	UI::_0x560E76D5E2E1803F(handle, 18, 1);
	UI::_0xF7AA2696A22AD8B9(this->handle); // _UIPROMPT_REGISTER_END

	semiHoldShouldReturn = false;
	isEnabled = true;
	isVisible = false;
}

Entity Prompt::getTargetEntity()
{
	return this->targetEntity;
}

bool Prompt::getIsEnabled()
{
	return isEnabled;
}

void Prompt::setText(const char* text)
{
	this->text = text;
	const char* textLabel = UI::_CREATE_VAR_STRING(10, "LITERAL_STRING", text);
	UI::_0x5DD02A8318420DD7(this->handle, (Any*)textLabel); // _UIPROMPT_SET_TEXT
}

void Prompt::setControl(Hash control)
{
	this->control = control;
	UI::_0xB5352B7494A08258(this->handle, control); // _UIPROMPT_SET_CONTROL_ACTION
}

Hash Prompt::getControl()
{
	return control;
}

void Prompt::setTargetEntity(Entity target)
{
	this->targetEntity = target;

	if (!target)
	{
		UI::_0x2F11D3A254169EA4(this->handle, 0, 0);
		return;
	}

	int targetEntityGroupId = UI::_0xB796970BD125FCE8(target); // _UIPROMPT_GET_GROUP_ID_FOR_TARGET_ENTITY 
	if (targetEntityGroupId)
	{
		UI::_0x2F11D3A254169EA4(this->handle, targetEntityGroupId, 0); // _UIPROMPT_SET_GROUP
	}
}

void Prompt::setGroup(int group)
{
	UI::_0x2F11D3A254169EA4(this->handle, group, 0); // _UIPROMPT_SET_GROUP

}

void Prompt::attachToCampfire()
{
	setGroup(1754796591); // Camp fire group
}

void Prompt::attachToRestingScenario()
{
	setGroup(342152817); // Resting scenario group
}

void Prompt::setPriority(int priority) 
{
	UI::_0xCA24F528D0D16289(handle, priority); // _UIPROMPT_SET_PRIORITY
}


void Prompt::setMode(PromptMode mode)
{
	this->mode = mode;

	switch (mode)
	{
	case Standard:
		UI::_0xCC6656799977741B(handle, 1); // UIPROMOT_SET_STANDARD_MODE
		break;

	case Hold:
		UI::_0x94073D5CA3F16B7B(handle, 1); // _UIPROMPT_SET_HOLD_MODE
		break;
	case SemiHold:
		UI::_0x94073D5CA3F16B7B(handle, 1); // _UIPROMPT_SET_HOLD_MODE
		break;
	case Mash:
		UI::_0x32DF729D8BD3C1C6(handle, 1, 0, 0, 0); // _UIPROMPT_SET_MASH_MANUAL_MODE
		break;
	}
}

bool Prompt::isActivatedByPlayer()
{
	if (!this->isEnabled || !this->isVisible)
	{
		return false;
	}

	switch (mode)
	{
	case Standard:
		return UI::_0xC92AC953F0A982AE(handle, 0); // _UIPROMPT_HAS_STANDARD_MODE_COMPLETED
	case Hold:
		return UI::_0xE0F65F0640EF0617(handle); // _UIPROMPT_HAS_HOLD_MODE_COMPLETED
	case SemiHold:
		if (UI::_0xE0F65F0640EF0617(handle))
		{
			if (semiHoldShouldReturn == true)
			{
				return false;
			}

			semiHoldShouldReturn = true;
			return true;
		}
		else
		{
			semiHoldShouldReturn = false;
		}
	case Mash:
		return UI::_0x845CE958416DC473(handle); // _UIPROMPT_HAS_MASH_MODE_COMPLETED
	}
}

bool Prompt::isControlPressed() {
	return CONTROLS::IS_DISABLED_CONTROL_PRESSED(0, control);
}

void Prompt::setIsEnabled(bool isEnabled)
{
	this->isEnabled = isEnabled;
	update();
}

void Prompt::show()
{
	isVisible = true;
	update();
}

void Prompt::hide()
{
	isVisible = false;
	update();
}

void Prompt::remove()
{
	UI::_0x00EDE88D4D13CF59(handle);
}

void Prompt::update()
{
	if (isVisible)
	{
		UI::_0x8A0FB4D03A630D21(this->handle, isEnabled); // _UIPROMPT_SET_ENABLED
		UI::_0x71215ACCFDE075EE(this->handle, true); // _UIPROMPT_SET_VISIBLE
	}
	else
	{
		UI::_0x8A0FB4D03A630D21(this->handle, false); // _UIPROMPT_SET_ENABLED
		UI::_0x71215ACCFDE075EE(this->handle, false); // _UIPROMPT_SET_VISIBLE
	}
}

float Prompt::getMashModeProgress()
{
	return UI::_0x8A9585293863B8A5(handle); // _UIPROMPT_GET_MASH_MODE_PROGRESS
}

void Prompt::setMashModeIncreasePerPress(float rate)
{
	UI::_0xA0D1D79C6036A855(handle, rate); //_UIPROMPT_SET_MASH_MANUAL_MODE_INCREASE_PER_PRESS

}

void Prompt::setMashModeGrowthSpeed(float speed)
{
	UI::_0x56DBB26F98582C29(handle, speed); //_UIPROMPT_SET_MASH_MANUAL_MODE_PRESSED_GROWTH_SPEED

}

void Prompt::setMashModeDecaySpeed(float speed)
{
	UI::_0x7D393C247FB9B431(handle, speed); // _UIPROMPT_SET_MASH_MANUAL_MODE_DECAY_SPEED
}

void Prompt::setUrgentPulsating(bool toggle)
{
	UI::_0xC5F428EE08FA7F2C(handle, toggle); // _UIPROMPT_SET_URGENT_PULSING_ENABLED
}

bool Prompt::isUrgentPulsating()
{
	return UI::_0x1FBA0DABECDDB52B(handle); // _UIPROMPT_GET_URGENT_PULSING_ENABLED
}
