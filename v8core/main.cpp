#include "core.h"

using namespace v8;
int main(int argc, char* argv[]) {
	init(argc, argv);
	start();
	stop();
	system("pause");
	return 0;
}