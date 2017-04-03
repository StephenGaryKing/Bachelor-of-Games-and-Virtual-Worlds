#include "Application2D.h"

int main() {
	
	auto app = new Application2D();
	//					  width	 height	 Fullscreen
	app->run("Game Of War", 720, 720, false);
	delete app;

	return 0;
}