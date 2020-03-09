#pragma once
#include <memory>
#include "v8.h"


extern v8::Local<v8::Context> NewContext(
	v8::Isolate* isolate,
	v8::Local<v8::ObjectTemplate> object_template =
	v8::Local<v8::ObjectTemplate>());

extern bool InitializeContext(v8::Local<v8::Context> context);

extern bool globalInitialize(const char* path, v8::Platform* platform );

extern void globalDispose();