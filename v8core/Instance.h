#pragma once
#include <memory>
#include "v8.h"
#include "libplatform/libplatform.h"
#include "api.h"

class Environment;

class Instance
{
public:
	Instance(v8::Isolate::CreateParams* params, v8::Platform* platform);
	~Instance();

private:
	std::unique_ptr<v8::ArrayBuffer::Allocator> array_buffer_allocator_;
	v8::Platform* platform_;
	v8::Isolate* isolate_;
	std::unique_ptr<Environment*> env_;
};

