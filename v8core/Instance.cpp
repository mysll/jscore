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
	isolate_->Dispose();
}

void Instance::Initialize(core_module* modlist_internal)
{
	HandleScope handle_scope(isolate_);
	Local<Context> context = NewContext(isolate_);
	Context::Scope context_scope(context);
	env_ = std::make_unique<Environment>(isolate_, context);
	env()->registerInternalModule(modlist_internal);
}

void Instance::run()
{
	Isolate::Scope isolate_scope(isolate_);
	HandleScope handle_scope(isolate_);
	Local<Context> context = NewContext(isolate_, env_->getTemplate());
	Context::Scope context_scope(context);
	env_->AssignToContext(context);
	env_->ExecuteFile("main.js");
}
