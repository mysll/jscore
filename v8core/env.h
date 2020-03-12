#pragma once
#include "v8.h"
#include <unordered_set>

class ScriptFile;
struct core_module;
class BaseObject;

class IsolateData {

};

class Environment
{
public:
	Environment(v8::Isolate* isolate, v8::Local<v8::Context> context);
	~Environment();
	void Dispose();

	inline v8::Isolate* isolate() const;
	inline v8::Local<v8::Context> context() const;

	static inline Environment* GetCurrent(v8::Isolate* isolate);
	static inline Environment* GetCurrent(v8::Local<v8::Context> context);

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

	inline void SetInstanceMethod(v8::Local<v8::FunctionTemplate> that,
		const char* name,
		v8::FunctionCallback callback);

	void registerInternalModule(v8::Local<v8::Object>& target, core_module* internals);

	bool ExecuteScript(const char * source);

	bool ExecuteFile(const char * filename);

	void set_console_template(v8::Local<v8::FunctionTemplate> tpl) { console_template_.Reset(isolate(), tpl); }

	v8::Local<v8::ObjectTemplate> getTemplate();

	void registerObject(BaseObject* obj);

	void unregisterObject(BaseObject* obj);

	void modifyObjectCount(int count);

private:
	bool runScript(ScriptFile* source);

private:

	std::unordered_set<BaseObject*> objects_;

	v8::Isolate* const isolate_;
	v8::Global<v8::Context> context_;
	v8::Global<v8::FunctionTemplate> console_template_;

	size_t count_;
};

