#include "core.h"
#include "api.h"
#include "v8.h"
#include "libplatform/libplatform.h"

using namespace v8;

struct V8Platform v8_platform;
static core_module* modlist_linked;

int init(int argc, char * argv[])
{
	modlist_linked = NULL;
	v8_platform.Initialize();
	GlobalInitialize(argv[0], v8_platform.platform());
	return 0;
}

int start()
{
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
	m->next = modlist_linked;
	modlist_linked = m;
}
