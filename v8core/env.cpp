#include "env.h"

using namespace v8;

Environment::Environment(Isolate* isolate, Context context):
	isolate_(isolate)
{
}

Environment::~Environment()
{
}
