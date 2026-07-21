#ifndef INPUTDEVICE_INCLUDED
#define INPUTDEVICE_INCLUDED

#include "GLFW/glfw3.h"

class InputDevice {
public:
	InputDevice();
	~InputDevice();

	void Init(GLFWwindow* window_src);
	void SetKeys();
	bool GetKeyPress(int code);
	void BeginTextInput(char* buffer, size_t size);

private:
	GLFWwindow* m_pWindow;

};

#endif
