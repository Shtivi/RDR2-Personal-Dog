#pragma once

class MissionTemplate 
{
public:
	int baseReward;
	MissionTemplateType type;
	const char* title;
	const char* description;
	HonorEffect honorEffect;

	static MissionTemplate* fromType(MissionTemplateType type);
	MissionTemplate(MissionTemplateType type, int baseReward, HonorEffect honorEffect, const char* title, const char* description = NULL);
	const char* baseRewardDisplay();
};