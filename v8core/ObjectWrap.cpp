#include "ObjectWrap.h"
using namespace v8;

BaseObject::BaseObject(Environment*env, Local<Object> handle):
	env_(env),
	handle_(env->isolate(), handle)
{
	assert(handle->InternalFieldCount() > 0);
	handle->SetAlignedPointerInInternalField(0, static_cast<void*>(this));
	handle_.SetWeak(this, WeakCallback, v8::WeakCallbackType::kParameter);
	env_->modifyObjectCount(1);
	env_->registerObject(this);
}

BaseObject::~BaseObject()
{
	if (persistent().IsEmpty()) {
		return;
	}
	persistent().ClearWeak();
	persistent().Reset();
	env_->modifyObjectCount(-1);
	env_->unregisterObject(this);
}

void BaseObject::DeleteMe()
{
	delete this;
}

//void BaseObject::assignMemory()
//{
//	size_ = SelfSize();
//	env_->isolate()->AdjustAmountOfExternalAllocatedMemory(size_);
//}
