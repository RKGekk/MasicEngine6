#pragma once

class KeyboardEvent {
public:
	enum class EventType { Press, Release, Invalid };
	KeyboardEvent();
	KeyboardEvent(const EventType type, const unsigned char key);
	~KeyboardEvent() = default;

	bool IsPress() const;
	bool IsRelease() const;
	bool IsValid() const;
	unsigned char GetKeyCode() const;

private:
	EventType m_type;
	unsigned char m_key;
};