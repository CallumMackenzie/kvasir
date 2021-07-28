#ifndef KVASIR_INPUT_H_
#define KVASIR_INPUT_H_ 1

#include <unordered_map>
#ifndef NO_USE_INCLUDES
#define GLFW_INCLUDE_NONE
#include "glfw3.h"
#undef GLFW_INCLUDE_NONE
#endif

namespace kvasir
{
	enum key
	{
		Unknown = -1,
		KeyA,
		KeyB,
		KeyC,
		KeyD,
		KeyE,
		KeyF,
		KeyG,
		KeyH,
		KeyI,
		KeyJ,
		KeyK,
		KeyL,
		KeyM,
		KeyN,
		KeyO,
		KeyP,
		KeyQ,
		KeyR,
		KeyS,
		KeyT,
		KeyU,
		KeyV,
		KeyW,
		KeyX,
		KeyY,
		KeyZ,
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
		CapsLock,
		ScrollLock,
		NumLock,
		PrintScreen,
		KeyCount,
		Dash = Hyphen,
		BackSpace = Backspace,
		BackSlash = Backslash,
		SemiColon = Semicolon,
		Return = Enter
	};
	enum keystate
	{
		Release,
		Press,
		Repeat,
		KeyUp = Release,
		KeyDown = Press,
		KeyHeld = Repeat,
		NoPress = -1,
		Stateless = NoPress,
		NoAction = NoPress
	};
	struct key_event
	{
		key key = Unknown;
		keystate action = NoPress;
	};
	struct input
	{
		static key get_glfw_key(int glk);
		static keystate get_glfw_kst(int kst);
		static int get_key(key k);
		static int get_kst(keystate kst);
	};

}

#endif