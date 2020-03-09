#pragma once
#include "env.h"
#include "ContextData.h"

inline v8::Isolate * Environment::isolate() const
{
	return isolate_;
}

inline void Environment::AssignToContext(v8::Local<v8::Context> context)
{
	context->SetAlignedPointerInEmbedderData(
		ContextEmbedderIndex::kEnvironment, this);
}

inline v8::Local<v8::FunctionTemplate> Environment::NewFunctionTemplate(
	v8::FunctionCallback callback,
	v8::Local<v8::Signature> signature,
	v8::ConstructorBehavior behavior,
	v8::SideEffectType side_effect)
{
	return v8::FunctionTemplate::New(isolate(), callback, v8::Local<v8::Object>(),
		signature, 0, behavior, side_effect);
}

inline void Environment::SetMethod(v8::Local<v8::Object> that, const char * name, v8::FunctionCallback callback)
{
	v8::Local<v8::Context> context = isolate()->GetCurrentContext();
	v8::Local<v8::Function> function =
		NewFunctionTemplate(callback, v8::Local<v8::Signature>(),
			v8::ConstructorBehavior::kThrow,
			v8::SideEffectType::kHasSideEffect)
		->GetFunction(context)
		.ToLocalChecked();
	// kInternalized strings are created in the old space.
	const v8::NewStringType type = v8::NewStringType::kInternalized;
	v8::Local<v8::String> name_string =
		v8::String::NewFromUtf8(isolate(), name, type).ToLocalChecked();
	that->Set(context, name_string, function).Check();
	function->SetName(name_string);
}

inline void Environment::SetProtoMethod(v8::Local<v8::FunctionTemplate> that, const char * name, v8::FunctionCallback callback)
{
	v8::Local<v8::Signature> signature = v8::Signature::New(isolate(), that);
	v8::Local<v8::FunctionTemplate> t =
		NewFunctionTemplate(callback, signature, v8::ConstructorBehavior::kThrow,
			v8::SideEffectType::kHasSideEffect);
	// kInternalized strings are created in the old space.
	const v8::NewStringType type = v8::NewStringType::kInternalized;
	v8::Local<v8::String> name_string =
		v8::String::NewFromUtf8(isolate(), name, type).ToLocalChecked();
	that->PrototypeTemplate()->Set(name_string, t);
	t->SetClassName(name_string);  // NODE_SET_PROTOTYPE_METHOD() compatibility.
}
