#include "ObjectWrap.h"
using namespace v8;

BaseObject::BaseObject(Environment*env, Local<Object> handle):
	env_(env),
	handle_(env->isolate(), handle)
{
	assert(handle->InternalFieldCount() > 0);
	handle->SetAlignedPointerInInternalField(0, this);
	handle_.SetWeak(this, WeakCallback, v8::WeakCallbackType::kParameter);
}

BaseObject::~BaseObject()
{
	if (persistent().IsEmpty()) {
		return;
	}

	assert(persistent().IsNearDeath());
	persistent().ClearWeak();
	persistent().Reset();

}

//void BaseObject::assignMemory()
//{
//	size_ = SelfSize();
//	env_->isolate()->AdjustAmountOfExternalAllocatedMemory(size_);
//}
