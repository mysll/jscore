#include <iostream>
#include "env_inl.h"
#include "ScriptFile.h"
#include "Utils.h"
#include "Console.h"
#include "core.h"


using namespace v8;

Environment::Environment(Isolate* isolate, v8::Local<v8::Context> context) :
	isolate_(isolate),
	context_(context->GetIsolate(), context)
{
	// NamedPropertyHandlerConfiguration 
	HandleScope scope(isolate);
	Context::Scope context_scope(context);
	AssignToContext(context);
}

Environment::~Environment()
{
}

void Environment::registerInternalModule(Local<Object>& target, core_module * internals)
{
	core_module * m = internals;
	while (m) {
		m->reg(target, context());
		m = m->next;
	}
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
	Local<FunctionTemplate> console_tpl = PersistentToLocal::Strong(console_template_);
	globals->Set(isolate(), "Console", console_tpl);
	return scope.Escape(globals);
}

void Environment::registerObject(BaseObject * obj)
{
	objects_.emplace(obj);
}

void Environment::unregisterObject(BaseObject * obj)
{
	objects_.erase(obj);
}

void Environment::modifyObjectCount(int count)
{
	count_ += count;
}


void Environment::Dispose()
{
	while (!objects_.empty()) {
		std::vector<BaseObject*> callbacks(objects_.begin(), objects_.end());
		for (BaseObject* cb : callbacks) {
			cb->DeleteMe();
			objects_.erase(cb);
		}
	}
}

bool Environment::runScript(ScriptFile* file)
{
	HandleScope scope(isolate());
	TryCatch try_catch(isolate());
	Local<Context> context = isolate()->GetCurrentContext();
	Context::Scope context_scope(context);
	ScriptOrigin origin(OneByteString(isolate(), file->fileName()));
	do {
		Handle<String> _source;
		if (!String::NewFromUtf8(isolate(), file->getContent(), NewStringType::kNormal).ToLocal(&_source)) {
			break;
		}

		Handle<Script> _script;
		if (!Script::Compile(context, _source, &origin).ToLocal(&_script)) {
			break;
		}

		Handle<Value> result;
		if (!_script->Run(context).ToLocal(&result)) {
			std::cerr << report_exception(isolate(), context, try_catch) << std::endl;
			break;
		}

		return true;
	} while (false);
	return false;
}
