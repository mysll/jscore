#pragma once
#include <memory>
#include "v8.h"
#include "libplatform/libplatform.h"
#include "api.h"

class Environment;
struct core_module;

class Instance
{
public:
	Instance(v8::Isolate::CreateParams* params, v8::Platform* platform);
	~Instance();

	void Initialize(core_module* modlist_internal);
	void run();

	inline Environment* env() const {
		return env_.get();
	};
private:
	std::unique_ptr<v8::ArrayBuffer::Allocator> array_buffer_allocator_;
	v8::Platform* platform_;
	v8::Isolate* isolate_;
	std::unique_ptr<Environment> env_;
};

