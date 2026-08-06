#ifndef INPUTDEVICE_INCLUDED
#define INPUTDEVICE_INCLUDED

#include "GLFW/glfw3.h"

class InputDevice {
public:
	// Keys for using as IDs in the buffer
	enum ID_KEYPRESS{
		IDKP_Z,
		IDKP_X,
		IDKP_C,
		IDKP_SHIFT,

		IDKP_UP,
		IDKP_DOWN,
		IDKP_LEFT,
		IDKP_RIGHT,
		
		IDKP_ESCAPE,
		IDKP_ENTER,
		IDKP_BACKSPACE,
		IDKP_SCREENSHOT,
	};

	enum MOVE_KEY {
		MK_UP = 1,
		MK_DOWN = 2,
		MK_LEFT = 4,
		MK_RIGHT = 8,
	};

	enum CONTROL_KEY {
		CK_Z = 1,
		CK_X = 2,
		CK_C = 4,
		CK_SHIFT = 8,
	};

public:
	InputDevice();
	~InputDevice();

	void Init(GLFWwindow* window_src);
	void SetKeys();
	bool GetKeyPress(int code);

	// Specific keys...
	void Update();
	bool GetOK(); // Z or Enter
	bool GetCancel(); // X or Escape
	uint8_t GetMoveStatus(); // <- v ^ ->
	uint8_t GetControlStatus(); // Z, X, C, Shift
	bool GetEscape(); // Escape
	bool GetScreenshot(); // F2


	void BeginTextInput(char* buffer, size_t size);
private:
	bool GetSinglePress(ID_KEYPRESS id);

private:
	GLFWwindow* m_pWindow;
	int m_PastKeyBuffer[IDKP_SCREENSHOT + 1];
	int m_KeyBuffer[IDKP_SCREENSHOT + 1];
};

#endif
