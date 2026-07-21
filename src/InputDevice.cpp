#include "InputDevice.hpp"
#include "cstdlib"
#include "cstdio"

struct text_input_t {
	char* buffer;
	size_t size;
};

void TextInput(GLFWwindow* win, unsigned int code) {
	printf("Key Input: %c, %d\n", code, code);
}

InputDevice::InputDevice() {

}

InputDevice::~InputDevice() {

}

void InputDevice::Init(GLFWwindow* window_src) {
	m_pWindow = window_src;
}

void InputDevice::SetKeys() {

}

bool InputDevice::GetKeyPress(int code) {
	return GLFW_PRESS == glfwGetKey(m_pWindow, code);
}

void InputDevice::BeginTextInput(char* buffer, size_t size) {

	glfwSetCharCallback(m_pWindow, TextInput);
}


