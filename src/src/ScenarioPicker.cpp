#include "Main.h"

int MenuAlpha = 220;
int ItemsInLine = 3;
int Lines = 2;
float ItemWidth = 0.08f;
float ItemHeight = 0.08f;
float ListPadding = 0.015;
float ListHeight = Lines * ItemHeight + 2 * ListPadding;
float MenuHeight = ListHeight + 0.02f;
float MenuWidth = ItemWidth * 3;
float MenuX = 0.5 - MenuWidth / 2;
float MenuY = 0.5 - MenuHeight / 2;

ScenarioPicker::ScenarioPicker()
{
	_isOpen = false;
	selectPrompt = new Prompt("Select", GAMEPLAY::GET_HASH_KEY("INPUT_FRONTEND_ACCEPT"));
	cancelPrompt = new Prompt("Cancel", GAMEPLAY::GET_HASH_KEY("INPUT_FRONTEND_CANCEL"));
	currentItem = 0;
	selection = NULL;
	_hasSelection = false;
}

void ScenarioPicker::update()
{
	if (!_isOpen)
	{
		selectPrompt->hide();
		cancelPrompt->hide();
		return;
	}

	selectPrompt->show();
	cancelPrompt->show();
	
	if (cancelPrompt->isActivatedByPlayer())
	{
		cancel();
		return;
	}

	draw();
}

void ScenarioPicker::open()
{
	_isOpen = true;
	selection = NULL;
	_hasSelection = false;
	currentItem = 0;
	//GAMEPLAY::SET_TIME_SCALE(0);
	//GRAPHICS::ANIMPOSTFX_PLAY("MissionChoice");
}

void ScenarioPicker::cancel()
{
	this->select(NULL);
}

void ScenarioPicker::select(const char* selection)
{
	this->selection = selection;
	close();
}

void ScenarioPicker::close()
{
	_hasSelection = true;
	_isOpen = false;
	//GAMEPLAY::SET_TIME_SCALE(1);
	//GRAPHICS::ANIMPOSTFX_STOP("MissionChoice");
}

bool ScenarioPicker::hasSelection()
{
	return _hasSelection;
}

const char* ScenarioPicker::getSelection()
{
	return selection;
}

void ScenarioPicker::draw()
{
	drawSprite("generic_textures", "inkroller_1a", MenuX, MenuY, MenuWidth, MenuHeight, 0, 0, 0, 0, MenuAlpha);
	
	drawItems();
	drawFooter();
}

void ScenarioPicker::drawItems()
{
	drawSprite("menu_textures", "scroller_left_top", MenuX, MenuY + ListPadding, MenuWidth * 0.9, 0.01f, 0, 255, 255, 255, 180);
	drawSprite("menu_textures", "scroller_left_bottom", MenuX, MenuY + ListHeight - ListPadding, MenuWidth * 0.9, 0.01f, 0, 255, 255, 255, 180);


}

void ScenarioPicker::drawFooter()
{
	//drawSprite("generic_textures", "list_item_h_line_narrow", MenuX + 0.01, MenuY + MenuHeight - 0.035, MenuWidth * 0.9, 0.002f, 0, 255, 255, 255, 255);
	drawText((char*)"Created by Shtivi", 0.5, MenuY + MenuHeight - 0.03, 255, 255, 255, MenuAlpha, true, 0.25, 0.25, "$title");
}