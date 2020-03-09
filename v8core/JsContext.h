#pragma once
#include <v8.h>
#include "ScriptFile.h"
class JsContext
{
public:
	JsContext(v8::Isolate* isolate, v8::Local<v8::ObjectTemplate> globals);
	~JsContext();

public:
	v8::Persistent<v8::Value> runWithFile(const char * file);
	v8::Persistent<v8::Value> runScript(const char * source, const char* filename = "no file");
	v8::Handle<v8::Context> getContext();

	bool initEnv();

private:
	v8::Persistent<v8::Value> runScript_(ScriptFile& file);

private:
	v8::Isolate* isolate_;
	v8::Persistent<v8::Context> context_;
};

