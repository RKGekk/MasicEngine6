#pragma once

struct MousePoint {
	int x;
	int y;
};

class MouseEvent {
public:
	enum class EventType {
		LPress,
		LRelease,
		RPress,
		RRelease,
		MPress,
		MRelease,
		WheelUp,
		WheelDown,
		Move,
		RAW_MOVE,
		Invalid
	};

	MouseEvent();
	MouseEvent(const EventType et, const int x, const int y);

	bool IsValid() const;
	EventType GetType() const;
	MousePoint GetPos() const;
	int GetPosX() const;
	int GetPosY() const;

private:
	EventType m_type;
	int m_pos_x;
	int m_pos_y;
};