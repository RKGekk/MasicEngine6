#pragma once

#include <memory>
#include <string>

#include <DirectXMath.h>

#include "render_component_interface.h"
#include "../tools/tinyxml/tinyxml.h"
#include "../events/evt_data_new_render_component.h"
#include "../events/evt_data_modified_render_component.h"
#include "../events/i_event_manager.h"

class BaseRenderComponent;
typedef BaseRenderComponent* WeakBaseRenderComponentPtr;

class BaseRenderComponent : public RenderComponentInterface {
protected:
    DirectX::XMFLOAT4 m_color;
    std::shared_ptr<SceneNode> m_pSceneNode;

public:
    virtual bool VInit(TiXmlElement* pData) override;
    virtual void VPostInit() override;
    virtual void VOnChanged() override;
    virtual TiXmlElement* VGenerateXml() override;

    const DirectX::XMFLOAT4& GetColor() const;
    void SetColor(DirectX::XMFLOAT4 color);
    void SetColorR(float color);
    void SetColorG(float color);
    void SetColorB(float color);
    void SetColorA(float color);

protected:
    virtual bool VDelegateInit(TiXmlElement* pData);
    virtual std::shared_ptr<SceneNode> VCreateSceneNode() = 0;
    DirectX::XMFLOAT4 LoadColor(TiXmlElement* pData);

    virtual TiXmlElement* VCreateBaseElement();
    virtual void VCreateInheritedXmlElements(TiXmlElement* pBaseElement) = 0;

private:
    virtual std::shared_ptr<SceneNode> VGetSceneNode() override;
};