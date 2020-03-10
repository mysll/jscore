#include "core.h"
#include "api.h"
#include "v8.h"
#include "libplatform/libplatform.h"
#include "Instance.h"
#include "env_inl.h"

using namespace v8;

struct V8Platform v8_platform;
static core_module* modlist_internal;
Isolate::CreateParams params;

#define NODE_BUILTIN_STANDARD_MODULES(V)                                       \
V(Console)

#define V(modname) void _register_##modname();
NODE_BUILTIN_STANDARD_MODULES(V)
#undef V



int init(int argc, char * argv[])
{
	modlist_internal = NULL;
	v8_platform.Initialize();
	GlobalInitialize(argv[0], v8_platform.platform());
	return 0;
}

int start()
{
	RegisterBuiltinModules();
	Instance instance(&params, v8_platform.platform());
	instance.Initialize();
	core_module * m = modlist_internal;
	while (m) {
		m->reg(instance.env()->context());
		m = m->next;
	}
	instance.run();
	return 0;
}

int stop()
{
	GlobalDispose();
	v8_platform.Dispose();
	return 0;
}

void module_register(core_module * m)
{
	m->next = modlist_internal;
	modlist_internal = m;
}

void RegisterBuiltinModules() {
#define V(modname) _register_##modname();
	NODE_BUILTIN_STANDARD_MODULES(V)
#undef V
}


