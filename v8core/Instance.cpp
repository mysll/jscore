#include "Instance.h"
#include "api.h"
#include "env.h"
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

void Instance::initEnv()
{
	HandleScope handle_scope(isolate_);
	Local<Context> context = NewContext(isolate_);
	Context::Scope context_scope(context);

	env_.reset(new Environment(isolate_, context));
}

void Instance::run()
{
	HandleScope handle_scope(isolate_);
	Local<Context> context = NewContext(isolate_, env_->getTemplate());
	Context::Scope context_scope(context);
	env_->AssignToContext(context);
	env_->ExecuteFile("main.js");
}
