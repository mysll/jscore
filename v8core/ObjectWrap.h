#pragma once
#include <v8.h>
#include <assert.h>
#include "env.h"

class ObjectWrap
{
public:
	ObjectWrap(Environment*env);
	virtual ~ObjectWrap();

	inline v8::Local<v8::Object> handle() { return handle(v8::Isolate::GetCurrent()); }
	inline v8::Local<v8::Object> handle(v8::Isolate* isolate) {
		return v8::Local<v8::Object>::New(isolate, persistent());
	}

	inline v8::Persistent<v8::Object>& persistent() { return handle_; }

	template <class T>
	static inline T* Unwrap(v8::Handle<v8::Object> handle) {
		assert(!handle.IsEmpty());
		assert(handle->InternalFieldCount() > 0);
		void* ptr = handle->GetAlignedPointerFromInternalField(0);
		ObjectWrap* wrap = static_cast<ObjectWrap*>(ptr);
		return static_cast<T*>(wrap);
	}

protected:
	inline void wrap(v8::Handle<v8::Object> handle) {
		assert(persistent().IsEmpty());
		assert(handle->InternalFieldCount() > 0);
		handle->SetAlignedPointerInInternalField(0, this);
		persistent().Reset(env_->isolate(), handle);
		MakeWeak();
	}

	inline void MakeWeak(void) {
		persistent().SetWeak(this, WeakCallback, v8::WeakCallbackType::kParameter);
		persistent().MarkIndependent();
	}

private:
	static void WeakCallback(const v8::WeakCallbackInfo<ObjectWrap>& data) {
		ObjectWrap* wrap = data.GetParameter();
		wrap->handle_.Reset();
		delete wrap;
	}
	v8::Persistent<v8::Object> handle_;
	Environment* env_;
};

