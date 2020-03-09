#pragma once
#include <v8.h>
#include <assert.h>

class ObjectWrap
{
public:
	ObjectWrap();
	~ObjectWrap();

	inline v8::Local<v8::Object> handle() { return handle(v8::Isolate::GetCurrent()); }
	inline v8::Local<v8::Object> handle(v8::Isolate* isolate) {
		return v8::Local<v8::Object>::New(isolate, persistent());
	}

	inline v8::Persistent<v8::Object>& persistent() { return handle_; }

protected:
	inline void wrap(v8::Handle<v8::Object> handle) {
		assert(persistent().IsEmpty());
		assert(handle->InternalFieldCount() > 0);
		handle->SetAlignedPointerInInternalField(0, this);
		persistent().Reset(v8::Isolate::GetCurrent(), handle);
	}

	inline void MakeWeak(void) {
		persistent().SetWeak(this, WeakCallback, v8::WeakCallbackType::kParameter);
		persistent().MarkIndependent();
	}

private:
	static void WeakCallback(const v8::WeakCallbackInfo<ObjectWrap>& data) {
		v8::Isolate* isolate = data.GetIsolate();

	}
	v8::Persistent<v8::Object> handle_;
};

