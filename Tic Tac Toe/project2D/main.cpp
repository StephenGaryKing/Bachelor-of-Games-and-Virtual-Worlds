#include "Application2D.h"

int main() {
	
	auto app = new Application2D();
	//					  width	 height	 Fullscreen
	app->run("Tic Tac Toe", 720, 800, false);
	delete app;

	return 0;
}