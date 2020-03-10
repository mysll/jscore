#include "env_inl.h"
#include "ScriptFile.h"
#include "Utils.h"
#include "Console.h"
using namespace v8;

Environment::Environment(Isolate* isolate, v8::Local<v8::Context> context) :
	isolate_(isolate)
{
	// NamedPropertyHandlerConfiguration 
	
	HandleScope scope(isolate);
	AssignToContext(context);
	Context::Scope context_scope(context);
	Console* console = new Console();
	console->initialize(this);
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
	Local<FunctionTemplate> console_tpl = PersistentToLocal::Weak(isolate(), console_template_);
	globals->Set(isolate(), "Console", console_tpl);
	return scope.Escape(globals);
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
