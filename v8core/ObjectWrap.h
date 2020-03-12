#pragma once
#include <v8.h>
#include <assert.h>
#include "env.h"

class ObjectWrap
{
public:
	ObjectWrap(Environment*env, v8::Local<v8::Object> object);
	virtual ~ObjectWrap();

	inline v8::Local<v8::Object> handle() { return handle(v8::Isolate::GetCurrent()); }
	inline v8::Local<v8::Object> handle(v8::Isolate* isolate) {
		return v8::Local<v8::Object>::New(isolate, persistent());
	}

	inline v8::Global<v8::Object>& persistent() { return handle_; }

	template <class T>
	static inline T* Unwrap(v8::Local<v8::Object> handle) {
		assert(!handle.IsEmpty());
		assert(handle->InternalFieldCount() > 0);
		void* ptr = handle->GetAlignedPointerFromInternalField(0);
		ObjectWrap* wrap = static_cast<ObjectWrap*>(ptr);
		return static_cast<T*>(wrap);
	}

private:
	static void WeakCallback(const v8::WeakCallbackInfo<ObjectWrap>& data) {
		ObjectWrap* wrap = data.GetParameter();
		wrap->handle_.Reset();
		delete wrap;
	}
	v8::Global<v8::Object> handle_;
	Environment* env_;
};

