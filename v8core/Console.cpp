#include "Console.h"
#include "env_inl.h"
#include "Utils_inl.h"

Console::Console()

{
}


Console::~Console()
{
}

//static void LogCallback(const FunctionCallbackInfo<Value>& args) {
//	Isolate*_isolate = args.GetIsolate();
//	HandleScope _scope(_isolate);
//	Handle<Value> arg = args[0];
//	String::Utf8Value value(_isolate, arg);
//	printf("%s\n", *value);
//}

bool Console::initialize(Environment* env)
{
	Local<Context> context = env->isolate()->GetCurrentContext();
	if (context.IsEmpty()) {
		return false;
	}

	Local<FunctionTemplate> ctor = env->NewFunctionTemplate(newConsole);
	ctor->SetClassName(FIXED_ONE_BYTE_STRING(env->isolate(), "Console"));

	env->SetProtoMethod(ctor, "info", info);
	env->SetProtoMethod(ctor, "log", log);

	ctor->InstanceTemplate()->SetInternalFieldCount(1);

	env->set_console_template(ctor);
	return true;
}

void Console::newConsole(const FunctionCallbackInfo<Value>& args)
{
	Isolate*_isolate = args.GetIsolate();
	Console* _console = new Console();
	Handle<Object> object = args.This();
	_console->wrap(object);
}

void Console::info(const FunctionCallbackInfo<Value>& args)
{
	Isolate*_isolate = args.GetIsolate();
	HandleScope _scope(_isolate);
	Handle<Value> arg = args[0];
	String::Utf8Value value(_isolate, arg);
	printf("[info]%s\n", *value);
}

void Console::log(const FunctionCallbackInfo<Value>& args)
{
	Isolate*_isolate = args.GetIsolate();
	HandleScope _scope(_isolate);
	Handle<Value> arg = args[0];
	String::Utf8Value value(_isolate, arg);
	printf("[log]%s\n", *value);
}
