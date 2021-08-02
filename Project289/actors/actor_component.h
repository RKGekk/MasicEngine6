#pragma once

#include <memory>
#include <functional>

#include "../tools/tinyxml/tinyxml.h"

#include "Actor.h"

class ActorComponent {
	friend class ActorFactory;

protected:
	StrongActorPtr m_pOwner;

public:
	virtual ~ActorComponent();

	virtual bool VInit(TiXmlElement* pData) = 0;
	virtual void VPostInit();
	virtual void VUpdate(float deltaMs);
	virtual void VOnChanged();

	virtual TiXmlElement* VGenerateXml() = 0;

	virtual ComponentId VGetId() const;
	virtual const std::string& VGetName() const = 0;
	static ComponentId GetIdFromName(const std::string& componentStr);

	void SetOwner(StrongActorPtr pOwner);
	StrongActorPtr GetOwner();
	ActorId GetOwnerId();
};