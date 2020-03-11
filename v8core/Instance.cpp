#include "Instance.h"
#include "api.h"
#include "env.h"
#include "core.h"
using namespace v8;

Instance::Instance(
	Isolate::CreateParams* params,
	Platform* platform) :
	array_buffer_allocator_(ArrayBuffer::Allocator::NewDefaultAllocator()),
	platform_(platform)
{
	params->array_buffer_allocator = array_buffer_allocator_.get();
	isolate_ = Isolate::Allocate();
	Isolate::Initialize(isolate_, *params);
}

Instance::~Instance()
{
	Dispose();
	isolate_->Exit();
	isolate_->Dispose();
}

void Instance::Initialize(core_module* modlist_internal)
{
	isolate_->Enter();
	env_ = CreateMainEnvironment(modlist_internal);
}

std::unique_ptr<Environment> Instance::CreateMainEnvironment(core_module* modlist_internal)
{
	HandleScope handle_scope(isolate_);
	Local<Context> context = NewContext(isolate_);
	Context::Scope context_scope(context);
	std::unique_ptr<Environment> env = std::make_unique<Environment>(isolate_, context);
	env->registerInternalModule(modlist_internal);
	return env;
}

void Instance::run()
{
	HandleScope handle_scope(isolate_);
	Local<Context> context = NewContext(isolate_, env()->getTemplate());
	Context::Scope context_scope(context);
	env()->AssignToContext(context);
	env()->ExecuteFile("main.js");
}

void Instance::Dispose()
{
	if (env() != nullptr) {
		env_->Dispose();
		env_.reset();
	}
}


