#include "env_inl.h"
#include "ScriptFile.h"
#include "Utils.h"
using namespace v8;

Environment::Environment(Isolate* isolate, v8::Local<v8::Context> context) :
	isolate_(isolate)
{
	// NamedPropertyHandlerConfiguration 
	AssignToContext(context);
}

Environment::~Environment()
{
}


bool Environment::ExecuteScript(const char * source)
{
	ScriptFile file("no file");
	if (!file.loadBySource(source)) {
		return false;
	}

	return runScript(&file);
}

bool Environment::ExecuteFile(const char * filename)
{
	ScriptFile file(filename);
	if (!file.load()) {
		return false;
	}

	return runScript(&file);
}

Local<ObjectTemplate> Environment::getTemplate()
{
	EscapableHandleScope scope(isolate());
	Local<ObjectTemplate> globals = ObjectTemplate::New(isolate());
	globals->Set(isolate(), "Console", PersistentToLocal::Weak(isolate(), console_template_));
	return globals;
}

bool Environment::runScript(ScriptFile* file)
{
	HandleScope scope(isolate());
	TryCatch try_catch(isolate_);
	Local<Context> context = isolate()->GetCurrentContext();
	Context::Scope context_scope(context);
	do {
		Handle<String> _source;
		if (!String::NewFromUtf8(isolate_, file->getContent(), NewStringType::kNormal).ToLocal(&_source)) {
			break;
		}

		Handle<Script> _script;
		if (!Script::Compile(context, _source).ToLocal(&_script)) {
			break;
		}

		Handle<Value> result;
		if (!_script->Run(context).ToLocal(&result)) {
			break;
		}

		return true;
	} while (false);

	return false;
}
