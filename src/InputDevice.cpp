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
	memset(m_KeyBuffer, 0x00, sizeof(m_KeyBuffer));
	memset(m_PastKeyBuffer, 0x00, sizeof(m_PastKeyBuffer));
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


void InputDevice::Update() {
	memcpy(m_PastKeyBuffer, m_KeyBuffer, sizeof(m_PastKeyBuffer));

	m_KeyBuffer[IDKP_Z] == glfwGetKey(m_pWindow, GLFW_KEY_Z);
	m_KeyBuffer[IDKP_X] == glfwGetKey(m_pWindow, GLFW_KEY_X);
	m_KeyBuffer[IDKP_C] == glfwGetKey(m_pWindow, GLFW_KEY_C);
	m_KeyBuffer[IDKP_SHIFT] == glfwGetKey(m_pWindow, GLFW_KEY_SHIFT);

	m_KeyBuffer[IDKP_UP] == glfwGetKey(m_pWindow, GLFW_KEY_UP);
	m_KeyBuffer[IDKP_DOWN] == glfwGetKey(m_pWindow, GLFW_KEY_DOWN);
	m_KeyBuffer[IDKP_LEFT] == glfwGetKey(m_pWindow, GLFW_KEY_LEFR);
	m_KeyBuffer[IDKP_RIGHT] == glfwGetKey(m_pWindow, GLFW_KEY_RIGHT);

	m_KeyBuffer[IDKP_ESCAPE] == glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE);
	m_KeyBuffer[IDKP_ENTER] == glfwGetKey(m_pWindow, GLFW_KEY_ENTER);
	m_KeyBuffer[IDKP_BACKSPACE] == glfwGetKey(m_pWindow, GLFW_KEY_BACKSPACE);
	m_KeyBuffer[IDKP_SCREENSHOT] == glfwGetKey(m_pWindow, GLFW_KEY_F2);
}

bool InputDevice::GetOK() {
	return GetSinglePress(IDKP_Z) || GetSinglePress(IDKP_ENTER);
}

bool InputDevice::GetCancel() {
	return GetSinglePress(IDKP_X) || GetSinglePress(IDKP_ESCAPE);
}

uint8_t InputDevice::GetMoveStatus() {
	return
		(GLFW_PRESS == m_KeyBuffer[IDKP_UP]) |
		((GLFW_PRESS == m_KeyBuffer[IDKP_DOWN]) << 1) |
		((GLFW_PRESS == m_KeyBuffer[IDKP_LEFT]) << 2) |
		((GLFW_PRESS == m_KeyBuffer[IDKP_RIGHT]) << 3)
	;
}

uint32_t InputDevice::GetControlStatus() {
	return
		(GLFW_PRESS == m_KeyBuffer[IDKP_Z]) |
		(GetSinglePress(IDKP_X) << 1) |
		(GetSinglePress(IDKP_C) << 2) |
		((GLFW_PRESS == m_KeyBuffer[IDKP_SHIFT]) << 3)
	;
}

bool InputDevice::GetEscape() {
	return GetSinglePress(IDKP_ESCAPE);
}

void InputDevice::BeginTextInput(char* buffer, size_t size) {

	glfwSetCharCallback(m_pWindow, TextInput);
}

bool InputDevice::GetSinglePress(ID_KEYPRESS id, int key_id) {
	return m_PastKeyBuffer[id] == GLFW_PRESS && glfwGetKey(m_pWindow, key_id);
}

