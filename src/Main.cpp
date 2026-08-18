#include "GameCore.hpp"

int main() {
	GameCore* core = new GameCore();

	if(nullptr == core) {
		printf("Out of memory\n");
		return -1;
	}

	if(false == core->InitGame()) {
		printf("Failed to initialize game\n");
		return -1;
	}
	return 0;
}
