#include "ObjectWrap.h"
using namespace v8;

BaseObject::BaseObject(Environment*env, Local<Object> handle):
	env_(env),
	handle_(env->isolate(), handle)
{
	assert(handle->InternalFieldCount() > 0);
	handle->SetAlignedPointerInInternalField(0, this);
	handle_.SetWeak(this, WeakCallback, v8::WeakCallbackType::kParameter);

	size_ = SelfSize();
	env_->isolate()->AdjustAmountOfExternalAllocatedMemory(size_);
}

BaseObject::~BaseObject()
{
	if (persistent().IsEmpty()) {
		return;
	}

	assert(persistent().IsNearDeath());
	persistent().ClearWeak();
	persistent().Reset();
	const int64_t len = static_cast<int64_t>(size_);
	env_->isolate()->AdjustAmountOfExternalAllocatedMemory(-len);
}
