#pragma once
#include "v8.h"
#include "libplatform/libplatform.h"
typedef void(*reg_func)();

struct Module 
{
	const char * name;
	reg_func reg;
};

struct V8Platform {
	std::unique_ptr<v8::Platform> platform_;
	inline void Initialize() {
		platform_ = v8::platform::NewDefaultPlatform();
	}

	inline v8::Platform* platform() {
		return platform_.get();
	}

	inline void Dispose() {
		platform_.reset();
	}
};

extern struct V8Platform v8_platform;

extern int init(int argc, char * argv[]);
extern int start();
extern int stop();

#define REGISTER_INTERNAL_MODULE()