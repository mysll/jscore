#pragma once
#include "v8.h"
#include "libplatform/libplatform.h"

typedef void(*reg_func)(v8::Local<v8::Object>, v8::Local<v8::Context>);

struct core_module 
{
	const char * name;
	reg_func reg;
	core_module* next;
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

extern void RegisterBuiltinModules();
extern int init(int argc, char * argv[]);
extern int start();
extern int stop();
extern void module_register(core_module* m);

#ifndef STRINGIFY
#define STRINGIFY(n) STRINGIFY_HELPER(n)
#define STRINGIFY_HELPER(n) #n
#endif

#define REGISTER_INTERNAL_MODULE_CPP(modname, regfunc)	\
static core_module _module = {							\
	STRINGIFY(modname),									\
	regfunc												\
};														\
void _register_##modname() {module_register(&_module);}