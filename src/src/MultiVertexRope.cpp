#include "Main.h";

MultiVertexRope::MultiVertexRope(AttachedRope* base)
{
	ropes.push_back(base);
}

Entity MultiVertexRope::getBase()
{
	return ropes.back()->getBase();
}

Entity MultiVertexRope::getAttached()
{
	return ropes.front()->getAttached();
}

bool MultiVertexRope::isExist()
{
	vector<AttachedRope*>::iterator ropesItr = ropes.begin();
	while (ropesItr != ropes.end())
	{
		if (!(*ropesItr)->isExist())
		{
			return false;
		}
		ropesItr++;
	}

	return true;
}

bool MultiVertexRope::canWind()
{	
	if (!isExist())
	{
		return false;
	}

	return ropes.front()->canWind();
}

bool MultiVertexRope::canUnwind()
{
	return ropes.front()->canUnwind();
}

void MultiVertexRope::startUnwinding()
{
	ropes.front()->startUnwinding();
}

void MultiVertexRope::stopUnwinding()
{
	ropes.front()->stopUnwinding();
}

void MultiVertexRope::pinTo(Vector3 pos)
{
	AttachedRope* lastRope = ropes.back();
	AttachedRope* rope = new AttachedRope(lastRope->getBase(), pos);
	ENTITY::SET_ENTITY_ROTATION(rope->getBase(), 90, 0, 90, 2, 1);
	ropes.push_back(rope);
}

void MultiVertexRope::startWinding()
{
	ropes.front()->startWinding();
}

void MultiVertexRope::stopWinding()
{
	ropes.front()->stopWinding();
}

int MultiVertexRope::update()
{
	return ropes.front()->update();
}