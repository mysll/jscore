#pragma once
#include "v8.h"
#include <unordered_set>

class ScriptFile;
struct core_module;
class BaseObject;

class IsolateData {

};

class CleanupHookCallback {
public:
	CleanupHookCallback(void(*fn)(void*),
		void* arg,
		uint64_t insertion_order_counter)
		: fn_(fn), arg_(arg), insertion_order_counter_(insertion_order_counter) {}

	// Only hashes `arg_`, since that is usually enough to identify the hook.
	struct Hash {
		inline size_t operator()(const CleanupHookCallback& cb) const;
	};

	// Compares by `fn_` and `arg_` being equal.
	struct Equal {
		inline bool operator()(const CleanupHookCallback& a,
			const CleanupHookCallback& b) const;
	};

	inline BaseObject* GetBaseObject() const;

private:
	friend class Environment;
	void(*fn_)(void*);
	void* arg_;

	// We keep track of the insertion order for these objects, so that we can
	// call the callbacks in reverse order when we are cleaning up.
	uint64_t insertion_order_counter_;
};

class Environment
{
public:
	Environment(v8::Isolate* isolate, v8::Local<v8::Context> context);
	~Environment();

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

	void Dispose();

private:
	bool runScript(ScriptFile* source);

private:

	std::unordered_set<CleanupHookCallback,
		CleanupHookCallback::Hash,
		CleanupHookCallback::Equal> cleanup_hooks_;

	v8::Isolate* const isolate_;
	v8::Global<v8::Context> context_;
	v8::Global<v8::FunctionTemplate> console_template_;
};

