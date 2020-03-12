#pragma once
#include <v8.h>
#include "ObjectWrap.h"
#include "env.h"

using namespace v8;
namespace console {
	class Console : BaseObject
	{
	public:
		Console(Environment*env, Handle<Object> handle);
		~Console();

		SET_SELF_SIZE(Console);

	public:
		static void ctor(const FunctionCallbackInfo<Value>& args);
		static void info(const FunctionCallbackInfo<Value>& args);
		static void log(const FunctionCallbackInfo<Value>& args);
	};
}

