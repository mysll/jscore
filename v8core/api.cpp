#include "api.h"
using namespace v8;

Local<Context> NewContext(Isolate* isolate,
	Local<ObjectTemplate> object_template) {
	auto context = Context::New(isolate, nullptr, object_template);
	if (context.IsEmpty()) return context;

	if (!InitializeContext(context)) {
		return Local<Context>();
	}

	return context;
}

bool InitializeContext(Local<Context> context) {
	return true;
}