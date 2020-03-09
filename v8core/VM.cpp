#include "VM.h"
#include <libplatform\libplatform.h>

#include "JsContext.h"

using namespace v8;

VM::VM(char * path):
	isolate_(NULL),
	platform_(platform::NewDefaultPlatform())
{
	if (!init(path)) {
		exit(0);
	}
}

VM::~VM()
{
	release();
}

bool VM::init(char * path)
{
	// init v8
	if (!V8::InitializeICUDefaultLocation(path)) {
		return false;
	}
	V8::InitializeExternalStartupData(path);
	V8::InitializePlatform(platform_.get());
	if (!V8::Initialize()) {
		return false;
	}

	return true;
}

void VM::release()
{
	V8::Dispose();
	V8::ShutdownPlatform();
	delete create_params_.array_buffer_allocator;
}

JsContext* VM::newContent(Local<ObjectTemplate> globals)
{
	JsContext* _context = new JsContext(isolate_, globals);
	return _context;
}

v8::Isolate * VM::getIsolate() const
{
	return isolate_;
}


