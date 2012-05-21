#include "WireButton.h"

namespace Wire
{

const char* GetButtonName(Button button)
{
	switch (button)
	{
	case BUTTON_1:
		return "BUTTON_1";
	case BUTTON_2:
		return "BUTTON_2";
	case BUTTON_A:
		return "BUTTON_A";
	case BUTTON_B:
		return "BUTTON_B";
	case BUTTON_Z:
		return "BUTTON_Z";
	case BUTTON_C:
		return "BUTTON_C";
	case BUTTON_PLUS:
		return "BUTTON_PLUS";
	case BUTTON_MINUS:
		return "BUTTON_MINUS";
	case BUTTON_HOME:
		return "BUTTON_HOME";
	default:
		return "INVALID_BUTTON";
	}
}

}
