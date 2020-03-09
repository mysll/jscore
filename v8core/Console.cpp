#include "Console.h"



Console::Console(Isolate* isolate) :
	isolate_(isolate)
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

bool Console::install(Isolate* isolate, Handle<ObjectTemplate> global)
{
	/*Handle<FunctionTemplate> fun = FunctionTemplate::New(isolate, LogCallback);
	global->Set(String::NewFromUtf8(isolate, "print", NewStringType::kNormal).ToLocalChecked(),
		fun);*/

	Handle<FunctionTemplate> _console_template = FunctionTemplate::New(isolate, Console::newConsole);
	Handle<ObjectTemplate> _console_proto = _console_template->PrototypeTemplate();
	_console_proto->Set(String::NewFromUtf8(isolate, "info", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate, info));
	_console_proto->Set(String::NewFromUtf8(isolate, "log", NewStringType::kNormal).ToLocalChecked(),
		FunctionTemplate::New(isolate, log));

	Handle<ObjectTemplate> _console_inst = _console_template->InstanceTemplate();
	_console_inst->SetInternalFieldCount(1);
	

	/*Handle<ObjectTemplate> global_console = ObjectTemplate::New(isolate);
	global_console->SetAccessor(String::NewFromUtf8(isolate, "console", NewStringType::kNormal))*/

	global->Set(String::NewFromUtf8(isolate, "Console", NewStringType::kNormal).ToLocalChecked(),
		_console_template);

	return true;
}

void Console::newConsole(const FunctionCallbackInfo<Value>& args)
{
	Isolate*_isolate = args.GetIsolate();
	Console* _console = new Console(_isolate);
	Handle<Object> object = args.This();
	object->SetInternalField(0, External::New(_isolate, _console));
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
