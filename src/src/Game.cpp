#include "Main.h"

void playMusic(const char* musicName)
{
	while (!AUDIO::PREPARE_MUSIC_EVENT((char*)musicName))
	{
		WAIT(10);
	}
	AUDIO::TRIGGER_MUSIC_EVENT((char*)musicName);
}

void stopMusic(const char* musicName)
{
	AUDIO::CANCEL_MUSIC_EVENT((char*)musicName);
}

const char* getKeyboardInput(const char* title, const char* subtitle, const char* placeholder, int maxInputLength)
{
	GAMEPLAY::DISPLAY_ONSCREEN_KEYBOARD(
		1,
		(char*)UI::_CREATE_VAR_STRING(10, "LITERAL_STRING", title), 
		(char*)UI::_CREATE_VAR_STRING(10, "LITERAL_STRING", subtitle), 
		(char*)placeholder, 
		"", 
		"", 
		"", 
		maxInputLength);
	
	int result = 0;
	while (!result)
	{
		result = GAMEPLAY::UPDATE_ONSCREEN_KEYBOARD();
		WAIT(0);
	}

	if (result != 1)
	{
		return NULL;
	}

	return GAMEPLAY::GET_ONSCREEN_KEYBOARD_RESULT();
}

int* getEventData(int eventIndex, int eventSize, int section)
{
	int* arr = new int[eventSize];

	if (!SCRIPT::GET_EVENT_DATA(section, eventIndex, arr, eventSize))
	{
		log("unexpected error: event has no data.");
		return NULL;
	}

	return arr;
}