#include "ObjectWrap.h"

ObjectWrap::ObjectWrap(Environment*env):env_(env)
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
