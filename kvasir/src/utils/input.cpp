#include "input.h"

using namespace kvasir;
kv_key input::glfw_key_to_kv_key(int glk)
{
	switch (glk)
	{
	default:
		break;
	}
	return Unknown;
}
kv_keystate input::glfw_kst_to_kv_kst(int kst);
{
	switch (kst)
	{
	case GLFW_PRESS:
		return Press;
	case GLFW_RELEASE:
		return Release;
	case GLFW_REPEAT:
		return Repeat;
	default:
		break;
	}
	return Unknown;
}