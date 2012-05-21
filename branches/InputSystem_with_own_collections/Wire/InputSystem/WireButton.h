#ifndef WIREBUTTON_H_
#define WIREBUTTON_H_

namespace Wire
{

enum Button
{
	BUTTON_A,
	BUTTON_B,
	BUTTON_1,
	BUTTON_2,
	BUTTON_Z,
	BUTTON_C,
	BUTTON_HOME,
	BUTTON_MINUS,
	BUTTON_PLUS,
	INVALID_BUTTON
};

const char* GetButtonName(Button button);

}

#endif
