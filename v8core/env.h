#pragma once
#include "v8.h"

class IsolateData {

};

class Environment
{
public:
	Environment(v8::Isolate* isolate, v8::Context context);
	~Environment();

	inline v8::Isolate* isolate() const;
private:
	v8::Isolate* const isolate_;
};

