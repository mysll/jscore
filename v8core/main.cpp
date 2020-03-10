#include "core.h"

using namespace v8;
int main(int argc, char* argv[]) {
	init(argc, argv);
	start();
	system("pause");
	stop();
	return 0;
}