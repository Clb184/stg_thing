#include "XPFW.h"

void Move(window_t* window, float dt, void* data) {

}

void Draw(window_t* window, float dt, void* data) {

}

int main() {
	window_t window;
	CreateGLWindow("Test", 1280, 960, false, &window);
	RunMainLoopDT(&window, nullptr, Move, Draw);
	return 0;
}
