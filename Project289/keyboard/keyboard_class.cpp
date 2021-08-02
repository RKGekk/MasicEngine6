#include "keyboard_class.h"

KeyboardClass::KeyboardClass() : m_key_states({false}) {}

bool KeyboardClass::KeyIsPressed(const unsigned char keycode) {
	return m_key_states[keycode];
}

bool KeyboardClass::KeyBufferIsEmpty() {
	return m_key_buffer.empty();
}

bool KeyboardClass::CharBufferIsEmpty() {
	return m_char_buffer.empty();
}

KeyboardEvent KeyboardClass::ReadKey() {
	if (m_key_buffer.empty()) {
		return KeyboardEvent();
	}
	else {
		KeyboardEvent e = m_key_buffer.front();
		m_key_buffer.pop();
		return e;
	}
}

unsigned char KeyboardClass::ReadChar() {
	if (m_char_buffer.empty()) {
		return 0u;
	}
	else {
		unsigned char e = m_char_buffer.front();
		m_char_buffer.pop();
		return e;
	}
}

void KeyboardClass::OnKeyPressed(const unsigned char key) {
	m_key_states[key] = true;
	m_key_buffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void KeyboardClass::OnKeyReleased(const unsigned char key) {
	m_key_states[key] = false;
	m_key_buffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void KeyboardClass::OnChar(const unsigned char key) {
	m_char_buffer.push(key);
}

void KeyboardClass::EnableAutoRepeatKeys() {
	m_auto_repeat_keys = true;
}

void KeyboardClass::DisableAutoRepeatKeys() {
	m_auto_repeat_keys = false;
}

void KeyboardClass::EnableAutoRepeatChars() {
	m_auto_repeat_chars = true;
}

void KeyboardClass::DisableAutoRepeatChars() {
	m_auto_repeat_chars = false;
}

bool KeyboardClass::IsKeyAutoRepeat() {
	return m_auto_repeat_keys;
}

bool KeyboardClass::IsCharAutoRepeat() {
	return m_auto_repeat_chars;
}
