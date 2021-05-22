#include "Main.h"

char rightText[200];
int rightTextDuration;

void clearRightText()
{
    for (int i = 0; i < 200; i++)
    {
        rightText[i] = 0;
    }
}

void setRightText(const char* text, int duration)
{
    int i = 0;
    while (i < 200 && text[i])
    {
        rightText[i] = text[i];
        i++;
    }

    rightTextDuration = duration;
}

DurationsArgs rightTextArgs1;
UITextArgs rightTextArgs2;

const char* createVarString(const char* text) 
{
    const char* result;
    result = UI::_CREATE_VAR_STRING(10, "LITERAL_STRING", text);
    int attempts = 500;
    while (strcmp(text, result) != 0 && attempts > 0)
    {
        log(attempts);
        attempts--;
        result = UI::_CREATE_VAR_STRING(10, "LITERAL_STRING", text);
    }
    log(attempts);
    log(result);
    return result;
}

void printRightText()
{
    if (rightText[0])
    {
        rightTextArgs1.duration = rightTextDuration;
        rightTextArgs2.unk = 0;
        rightTextArgs2.text = UI::_CREATE_VAR_STRING(10, "LITERAL_STRING", rightText);

        UIUNK::_0xB2920B9760F0F36B((Any*)&rightTextArgs1, (Any*)&rightTextArgs2, 1);
        clearRightText();
    }
}