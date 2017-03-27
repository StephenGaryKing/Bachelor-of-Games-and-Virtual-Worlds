#include "Testing2DThingsApp.h"

int main() {
	
	auto app = new Testing2DThingsApp();
	app->run("AIE", 1280, 720, false);
	delete app;

	return 0;
}