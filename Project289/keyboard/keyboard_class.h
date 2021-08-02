#pragma once

#include <queue>
#include <array>

#include "keyboard_event.h"

class KeyboardClass {
public:
	KeyboardClass();
	bool KeyIsPressed(const unsigned char keycode);
	bool KeyBufferIsEmpty();
	bool CharBufferIsEmpty();
	KeyboardEvent ReadKey();
	unsigned char ReadChar();
	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void OnChar(const unsigned char key);
	void EnableAutoRepeatKeys();
	void DisableAutoRepeatKeys();
	void EnableAutoRepeatChars();
	void DisableAutoRepeatChars();
	bool IsKeyAutoRepeat();
	bool IsCharAutoRepeat();

private:
	bool m_auto_repeat_keys = false;
	bool m_auto_repeat_chars = false;
	std::array<bool, 256> m_key_states;
	std::queue<KeyboardEvent> m_key_buffer;
	std::queue<unsigned char> m_char_buffer;
};