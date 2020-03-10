#include "Console.h"
#include "env_inl.h"
#include "Utils_inl.h"
#include "core.h"

namespace console {
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

	void Console::newConsole(const FunctionCallbackInfo<Value>& args)
	{
		Isolate*_isolate = args.GetIsolate();
		HandleScope scope(_isolate);
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


	void Initialize(v8::Local<v8::Context> context) {
		Environment* env = Environment::GetCurrent(context);
		if (env == nullptr) {
			return;
		}

		HandleScope scope(env->isolate());
		Context::Scope context_scope(context);

		Local<FunctionTemplate> ctor = env->NewFunctionTemplate(Console::newConsole);
		ctor->SetClassName(FIXED_ONE_BYTE_STRING(env->isolate(), "Console"));
		env->SetProtoMethod(ctor, "info", Console::info);
		env->SetProtoMethod(ctor, "log", Console::log);
		ctor->InstanceTemplate()->SetInternalFieldCount(1);

		env->set_console_template(ctor);
	}
}
REGISTER_INTERNAL_MODULE_CPP(Console, console::Initialize);