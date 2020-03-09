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

bool globalInitialize(const char* path, v8::Platform* platform)
{
	// init v8
	if (!V8::InitializeICUDefaultLocation(path)) {
		return false;
	}
	V8::InitializeExternalStartupData(path);
	V8::InitializePlatform(platform);
	if (!V8::Initialize()) {
		return false;
	}
	return true;
}

void globalDispose()
{
	V8::Dispose();
	V8::ShutdownPlatform();
}
