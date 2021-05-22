#include "Main.h";

Blip createBlip(Vector3 pos, Hash blipType, Hash blipSprite)
{
	Blip blip;
	blip = RADAR::_0x554D9D53F696D002(blipType, pos.x, pos.y, pos.z);
	if (blipSprite != 0)
	{
		RADAR::SET_BLIP_SPRITE(blip, blipSprite, false);
	}

	return blip;
}

Blip createBlip(Entity entity, Hash blipType, Hash blipSprite)
{
	Blip blip = RADAR::_0x23F74C2FDA6E7C61(blipType, entity); 

	if (blipSprite != 0)
	{
		RADAR::SET_BLIP_SPRITE(blip, blipSprite, false);
	}

	return blip;
}

Blip createBlip(Vector3 source, float radius, Hash blipType, Hash blipSprite)
{
	Blip blip = RADAR::_0x45F13B7E0A15C880(blipType, source.x, source.y, source.z, radius); // add blip for area

	if (blipSprite != 0)
	{
		RADAR::SET_BLIP_SPRITE(blip, blipSprite, false);
	}

	return blip;
}

void setBlipLabel(Blip blip, const char* label)
{
	RADAR::_0x9CB1A1623062F402(blip, (Any*)UI::_CREATE_VAR_STRING(10, "LITERAL_STRING", label)); // _SET_BLIP_NAME_FROM_PLAYER_STRING
}

void deleteBlipSafe(Blip* blip)
{
	if (RADAR::DOES_BLIP_EXIST(*blip))
	{
		RADAR::REMOVE_BLIP(blip);
	}
}