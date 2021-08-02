#include "mouse_class.h"

void MouseClass::OnLeftPressed(int x, int y) {
	m_left_is_down = true;
	MouseEvent e(MouseEvent::EventType::LPress, x, y);
	m_event_buffer.push(e);
}

void MouseClass::OnLeftReleased(int x, int y) {
	m_left_is_down = false;
	MouseEvent e(MouseEvent::EventType::LRelease, x, y);
	m_event_buffer.push(e);
}

void MouseClass::OnRightPressed(int x, int y) {
	m_right_is_down = true;
	MouseEvent e(MouseEvent::EventType::RPress, x, y);
	m_event_buffer.push(e);
}

void MouseClass::OnRightReleased(int x, int y) {
	m_right_is_down = false;
	MouseEvent e(MouseEvent::EventType::RRelease, x, y);
	m_event_buffer.push(e);
}

void MouseClass::OnMiddlePressed(int x, int y) {
	m_middle_is_down = true;
	MouseEvent e(MouseEvent::EventType::MPress, x, y);
	m_event_buffer.push(e);
}

void MouseClass::OnMiddleReleased(int x, int y) {
	m_middle_is_down = false;
	MouseEvent e(MouseEvent::EventType::MRelease, x, y);
	m_event_buffer.push(e);
}

void MouseClass::OnWheelUp(int x, int y) {
	MouseEvent e(MouseEvent::EventType::WheelUp, x, y);
	m_event_buffer.push(e);
}

void MouseClass::OnWheelDown(int x, int y) {
	MouseEvent e(MouseEvent::EventType::WheelDown, x, y);
	m_event_buffer.push(e);
}

void MouseClass::OnMouseMove(int x, int y) {
	m_pos_x = x;
	m_pos_y = y;
	MouseEvent e(MouseEvent::EventType::Move, x, y);
	m_event_buffer.push(e);
}

void MouseClass::OnMouseMoveRaw(int x, int y) {
	MouseEvent e(MouseEvent::EventType::RAW_MOVE, x, y);
	m_event_buffer.push(e);
}

bool MouseClass::IsLeftDown() {
	return m_left_is_down;
}

bool MouseClass::IsMiddleDown() {
	return m_middle_is_down;
}

bool MouseClass::IsRightDown() {
	return m_right_is_down;
}

int MouseClass::GetPosX() {
	return m_pos_x;
}

int MouseClass::GetPosY() {
	return m_pos_y;
}

MousePoint MouseClass::GetPos() {
	return { m_pos_x, m_pos_y };
}

bool MouseClass::EventBufferIsEmpty() {
	return m_event_buffer.empty();
}

MouseEvent MouseClass::ReadEvent() {
	if (m_event_buffer.empty()) {
		return MouseEvent();
	}
	else {
		MouseEvent e = m_event_buffer.front();
		m_event_buffer.pop();
		return e;
	}
}