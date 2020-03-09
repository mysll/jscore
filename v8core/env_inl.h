#pragma once
#include "env.h"

inline v8::Isolate * Environment::isolate() const
{
	return isolate_;
}