#include <libplatform\libplatform.h>
#include "JsContext.h"
#include "Console.h"
#include "api.h"
#include "Instance.h"


using namespace v8;

int start(Platform* platform) {
	Isolate::CreateParams params;
	std::unique_ptr<Instance> instance = std::make_unique<Instance>(&params, platform);
	instance->initEnv();
	instance->run();
	return 0;
}

int main(int argc, char* argv[]) {
	std::unique_ptr<Platform> platform = platform::NewDefaultPlatform();
	globalInitialize(argv[0], platform.get());
	start(platform.get());
	system("pause");
	globalDispose();
	return 0;
}