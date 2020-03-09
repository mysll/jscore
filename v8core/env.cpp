#include "env.h"

using namespace v8;

Environment::Environment(Isolate* isolate, v8::Local<v8::Context> context):
	isolate_(isolate)
{

}

Environment::~Environment()
{
}
