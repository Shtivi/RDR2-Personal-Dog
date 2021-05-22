#pragma once

class ScenarioPicker
{
private:
	bool _isOpen;
	Prompt* selectPrompt;
	Prompt* cancelPrompt;
	int currentItem;
	const char* selection;
	bool _hasSelection;

public:
	ScenarioPicker();
	void update();
	void open();
	bool hasSelection();
	const char* getSelection();

private:
	void cancel();
	void select(const char* selection);
	void close();
	void draw();
	
	void drawItems();
	void drawFooter();
};