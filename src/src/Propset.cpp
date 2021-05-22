#include "Main.h"

Propset::Propset(int handle)
{
	this->handle = handle;
}

Propset Propset::spawn(Vector3 location, const char* model)
{
	int propsetHash = GAMEPLAY::GET_HASH_KEY((char*)model);
	PROP::_0xF3DE57A46D5585E9(propsetHash);

	int i = 0;
	while (!PROP::_0x48A88FC684C55FDC(propsetHash) && i < 100)
	{
		i++;
		PROP::_0xF3DE57A46D5585E9(propsetHash);
		WAIT(20);
	}

	Propset propset(0);
	if (PROP::_0x48A88FC684C55FDC(propsetHash))
	{
		propset.handle = PROP::_0x899C97A1CCE7D483(propsetHash, location.x, location.y, location.z, 0, 60, 1200, false, true);
	}

	PROP::_0xB1964A83B345B4AB(propsetHash);
}