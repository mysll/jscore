#include "JsContext.h"
#include "ScriptFile.h"
#include "Console.h"

using namespace v8;

JsContext::JsContext(Isolate* isolate, Local<ObjectTemplate> globals)
{
	isolate_ = isolate;
	HandleScope _scope(isolate_);
	isolate_->SetCaptureStackTraceForUncaughtExceptions(true);
	if (globals.IsEmpty()) {
		globals = ObjectTemplate::New(isolate_);
	}
	Console::install(isolate, globals);
	context_.Reset(isolate, Context::New(isolate_, nullptr, globals));
}

JsContext::~JsContext()
{
	context_.Reset();
}

Persistent<Value> JsContext::runWithFile(const char * file)
{
	ScriptFile _sourceFile(file);
	if (!_sourceFile.load()) {
		return Persistent<Value>();
	}
	return runScript_(_sourceFile);
}

Persistent<Value> JsContext::runScript(const char * source, const char* filename)
{
	ScriptFile _sourceFile(filename);
	if (!_sourceFile.loadBySource(source)) {
		return Persistent<Value>();
	}

	return runScript_(_sourceFile);
}

v8::Handle<v8::Context> JsContext::getContext()
{
	return context_.Get(isolate_);
}

bool JsContext::initEnv()
{
	//Local<Object> _console = 
	return true;
}

Persistent<Value> JsContext::runScript_(ScriptFile & file)
{
	HandleScope _scope(isolate_);
	TryCatch try_catch(isolate_);
	Handle<Context> _context = context_.Get(isolate_);
	Context::Scope _contextScope(_context);
	do {
		Handle<String> _source;
		if (!String::NewFromUtf8(isolate_, file.getContent(), NewStringType::kNormal).ToLocal(&_source)) {
			break;
		}

		Handle<Script> _script;
		if (!Script::Compile(_context, _source).ToLocal(&_script)) {
			break;
		}

		Handle<Value> result;
		if (!_script->Run(_context).ToLocal(&result)) {
			break;
		}

		return Persistent<Value>(isolate_, result);
	} while (false);

	return Persistent<Value>(isolate_, try_catch.Exception());

}
