#pragma once
#include <v8.h>

using namespace v8;
class Console
{
public:
	Console(Isolate* isolate);
	~Console();

public:
	static bool  install(Isolate* isolate, Handle<ObjectTemplate> global);
	static void  newConsole(const FunctionCallbackInfo<Value>& args);
	static void info(const FunctionCallbackInfo<Value>& args);
	static void log(const FunctionCallbackInfo<Value>& args);

private:
	Isolate* isolate_;
	static Persistent<ObjectTemplate> inst_;
};

