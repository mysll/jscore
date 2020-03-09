#include "env_inl.h"

using namespace v8;

Environment::Environment(Isolate* isolate, v8::Local<v8::Context> context):
	isolate_(isolate)
{
	// NamedPropertyHandlerConfiguration
	AssignToContext(context);
}

Environment::~Environment()
{
}
