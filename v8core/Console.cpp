#include "Console.h"
#include "env_inl.h"
#include "Utils_inl.h"
#include "core.h"


Console::Console(Environment* env, Handle<Object> handle) :BaseObject(env, handle)
{
}

Console::~Console()
{
}

void Console::ctor(const FunctionCallbackInfo<Value>& args)
{
	Isolate*_isolate = args.GetIsolate();
	Environment* env = Environment::GetCurrent(_isolate);
	HandleScope scope(_isolate);
	Context::Scope context_scope(_isolate->GetCurrentContext());
	Handle<Object> object = args.This();
	Console* _console = new Console(env, object);
}

void Console::info(const FunctionCallbackInfo<Value>& args)
{
	Isolate*_isolate = args.GetIsolate();
	HandleScope _scope(_isolate);
	Handle<Value> arg = args[0];
	String::Utf8Value value(_isolate, arg);
	Console* _console = BaseObject::Unwrap<Console>(args.This());
	printf("[info]%s\n", *value);
}

void Console::log(const FunctionCallbackInfo<Value>& args)
{
	Isolate*_isolate = args.GetIsolate();
	HandleScope _scope(_isolate);
	Handle<Value> arg = args[0];
	String::Utf8Value value(_isolate, arg);
	Console* _console = BaseObject::Unwrap<Console>(args.This());
	printf("[log]%s\n", *value);
}

void Initialize(v8::Local<v8::Object> target, v8::Local<v8::Context> context) {
	Environment* env = Environment::GetCurrent(context);
	if (env == nullptr) {
		return;
	}

	HandleScope scope(env->isolate());
	Context::Scope context_scope(context);


	Local<FunctionTemplate> ctor = env->NewFunctionTemplate(Console::ctor);
	ctor->SetClassName(FIXED_ONE_BYTE_STRING(env->isolate(), "Console"));
	ctor->InstanceTemplate()->SetInternalFieldCount(1);
	env->SetProtoMethod(ctor, "info", Console::info);
	env->SetProtoMethod(ctor, "log", Console::log);

	Local<Object> instance = ctor->GetFunction(context).ToLocalChecked()->NewInstance(context).ToLocalChecked();
	target->Set(
		context,
		FIXED_ONE_BYTE_STRING(env->isolate(), "console"),
		instance
	);

	env->set_console_template(ctor);
}

REGISTER_INTERNAL_MODULE_CPP(Console, Initialize);