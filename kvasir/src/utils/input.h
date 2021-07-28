#ifndef KVASIR_INPUT_H_
#define KVASIR_INPUT_H_ 1

#include <unordered_map>
#ifndef NO_USE_INCLUDES
#include "glfw-window.h"
#endif

namespace kvasir
{
	enum kv_key
	{
		Unknown = -1,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		Num0,
		Num1,
		Num2,
		Num3,
		Num4,
		Num5,
		Num6,
		Num7,
		Num8,
		Num9,
		Escape,
		LControl,
		LShift,
		LAlt,
		LSystem,
		RControl,
		RShift,
		RAlt,
		RSystem,
		Menu,
		LBracket,
		RBracket,
		Semicolon,
		Comma,
		Period,
		Quote,
		Slash,
		Backslash,
		Tilde,
		Equal,
		Hyphen,
		Space,
		Enter,
		Backspace,
		Tab,
		PageUp,
		PageDown,
		End,
		Home,
		Insert,
		Delete,
		Add,
		Subtract,
		Multiply,
		Divide,
		Left,
		Right,
		Up,
		Down,
		Numpad0,
		Numpad1,
		Numpad2,
		Numpad3,
		Numpad4,
		Numpad5,
		Numpad6,
		Numpad7,
		Numpad8,
		Numpad9,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		F13,
		F14,
		F15,
		Pause,
		KeyCount,
		Dash = Hyphen,
		BackSpace = Backspace,
		BackSlash = Backslash,
		SemiColon = Semicolon,
		Return = Enter
	};
	enum kv_keystate
	{
		Release,
		Press,
		Repeat,
		Unknown = -1,
		KeyUp = Release,
		KeyDown = Press,
		KeyHeld = Repeat
	};
	struct key_event
	{
		kv_key key = Unknown;
		kv_keystate action = ;
	};
	struct input
	{
		static kv_key glfw_key_to_kv_key(int glk);
		static kv_keystate glfw_kst_to_kv_kst(int kst);
	};

}

#endif