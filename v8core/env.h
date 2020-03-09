#pragma once
#include "v8.h"

class IsolateData {

};

class Environment
{
public:
	Environment(v8::Isolate* isolate, v8::Local<v8::Context> context);
	~Environment();

	inline v8::Isolate* isolate() const;

	inline void AssignToContext(v8::Local<v8::Context> context);

	inline v8::Local<v8::FunctionTemplate>
		NewFunctionTemplate(v8::FunctionCallback callback,
			v8::Local<v8::Signature> signature =
			v8::Local<v8::Signature>(),
			v8::ConstructorBehavior behavior =
			v8::ConstructorBehavior::kAllow,
			v8::SideEffectType side_effect =
			v8::SideEffectType::kHasSideEffect);

	inline void SetMethod(v8::Local<v8::Object> that,
		const char* name,
		v8::FunctionCallback callback);

	inline void SetProtoMethod(v8::Local<v8::FunctionTemplate> that,
		const char* name,
		v8::FunctionCallback callback);
private:
	v8::Isolate* const isolate_;
};

