#include "Instance.h"
#include "api.h"
#include "env.h"
#include "core.h"
#include "Utils.h"
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
	isolate_->TerminateExecution();
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
	Local<Object> global = context->Global();
	env->registerInternalModule(global, modlist_internal);
	// TODO(joyeecheung): this can be done in JS land now.
	global->Set(context, FIXED_ONE_BYTE_STRING(isolate_, "global"), global)
		.Check();
	return env;
}

void Instance::run()
{
	{
		HandleScope handle_scope(isolate_);
		Local<Context> context = env()->context();
		Context::Scope context_scope(context);
		env()->AssignToContext(context);
		env()->ExecuteFile("main.js");
	}
	// test gc
	isolate_->AdjustAmountOfExternalAllocatedMemory(1024 * 1024*1024);
}

void Instance::Dispose()
{
	if (env() != nullptr) {
		env_->Dispose();
		env_.reset();
	}
}


