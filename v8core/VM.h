#pragma once
#include <v8.h>

class JsContext;

class VM
{
public:
	VM(char * path);
	~VM();

private:
	bool init(char * path);
	void release();

public:
	JsContext* newContent(v8::Local<v8::ObjectTemplate> globals);
	v8::Isolate* getIsolate() const;

private:
	std::unique_ptr<v8::Platform> platform_;
	v8::Isolate* isolate_;
	v8::Isolate::CreateParams create_params_;
};

