#pragma once
#include <v8.h>
#include "ObjectWrap.h"
#include "env.h"

using namespace v8;
namespace console {
	class Console : ObjectWrap
	{
	public:
		Console(Environment*env);
		~Console();

	public:
		static void  newConsole(const FunctionCallbackInfo<Value>& args);
		static void info(const FunctionCallbackInfo<Value>& args);
		static void log(const FunctionCallbackInfo<Value>& args);
	};
}

