#pragma once

#include <iostream>

#include "base_event_data.h"

class EvtData_Update_Tick : public BaseEventData {
	float m_DeltaSeconds;
	float m_TotalSeconds;

public:
	static const EventTypeId sk_EventType = 0xf0f5d183;
	static const std::string sk_EventName;

	explicit EvtData_Update_Tick(float deltaSeconds, float totalSeconds);

	virtual EventTypeId VGetEventType() const override;
	virtual IEventDataPtr VCopy() const override;
	virtual void VSerialize(std::ostream& out) const override;
	virtual const std::string& GetName() const override;

	friend std::ostream& operator<<(std::ostream& os, const EvtData_Update_Tick& evt);
};