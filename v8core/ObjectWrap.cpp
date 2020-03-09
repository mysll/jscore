#include "ObjectWrap.h"

ObjectWrap::ObjectWrap()
{
}


ObjectWrap::~ObjectWrap()
{
	if (persistent().IsEmpty()) {
		return;
	}

	assert(persistent().IsNearDeath());
	persistent().ClearWeak();
	persistent().Reset();
}
