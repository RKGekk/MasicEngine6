#include "mouse_event.h"

MouseEvent::MouseEvent() : m_type(MouseEvent::EventType::Invalid), m_pos_x(0), m_pos_y(0) {}

MouseEvent::MouseEvent(const EventType et, const int x, const int y) : m_type(et), m_pos_x(x), m_pos_y(y) {}

bool MouseEvent::IsValid() const {
	return m_type != MouseEvent::EventType::Invalid;
}

MouseEvent::EventType MouseEvent::GetType() const {
	return m_type;
}

MousePoint MouseEvent::GetPos() const {
	return {m_pos_x, m_pos_y};
}

int MouseEvent::GetPosX() const {
	return m_pos_x;
}

int MouseEvent::GetPosY() const {
	return m_pos_y;
}
