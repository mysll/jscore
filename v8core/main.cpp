#include "VM.h"
#include "JsContext.h"
#include "Console.h"

using namespace v8;
int main(int argc, char* argv[]) {

	VM vm(argv[0]);
	do {
		HandleScope _scope(vm.getIsolate());
		Local<ObjectTemplate> global = ObjectTemplate::New(vm.getIsolate());
		auto _context = vm.newContent(global);
		Persistent<Value> _result  = _context->runWithFile("main.js");
		if (_result.IsEmpty()) {
			printf("is empty\n");
			break;
		}
		String::Utf8Value utf8(vm.getIsolate(), _result.Get(vm.getIsolate()));
		printf("%s\n", *utf8);
		_result.Reset();
		delete _context;
	} while (false);
	do {
		HandleScope _scope(vm.getIsolate());
		auto _context = vm.newContent(Local<ObjectTemplate>());
		Persistent<Value> _result = _context->runScript("'hello'+',world'");
		if (_result.IsEmpty()) {
			printf("is empty\n");
			break;
		}
		String::Utf8Value utf8(vm.getIsolate(), _result.Get(vm.getIsolate()));
		printf("%s\n", *utf8);
		_result.Reset();
		delete _context;
	} while (false);
	system("pause");
	return 0;
}