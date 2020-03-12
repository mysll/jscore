#pragma once
#include <v8.h>
#include <assert.h>
#include "env.h"

// Set the self size of a MemoryRetainer to the stack-allocated size of a
// certain class
#define SET_SELF_SIZE(Klass)                                                   \
  inline size_t SelfSize() const override { return sizeof(Klass); }

class BaseObject
{
public:
	BaseObject(Environment*env, v8::Local<v8::Object> object);
	virtual ~BaseObject();

	virtual size_t SelfSize() const = 0;

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
		BaseObject* wrap = static_cast<BaseObject*>(ptr);
		return static_cast<T*>(wrap);
	}

	void DeleteMe();

private:
	static void WeakCallback(const v8::WeakCallbackInfo<BaseObject>& data) {
		BaseObject* wrap = data.GetParameter();
		wrap->DeleteMe();
	}
	v8::Global<v8::Object> handle_;
	Environment* env_;
	size_t size_;
};

