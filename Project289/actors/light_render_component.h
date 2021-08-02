#pragma once

#include <memory>
#include <string>

#include "base_render_component.h"
#include "../nodes/light_properties.h"
#include "transform_component.h"
#include "../tools/memory_utility.h"

class LightRenderComponent : public BaseRenderComponent {
    LightProperties m_Props;

public:
    static const std::string g_Name;
    virtual const std::string& VGetName() const override;

    LightRenderComponent();

    const LightProperties& GetLight() const;

    void SetAmbient4f(const DirectX::XMFLOAT4& color);
    void SetDiffuse4f(const DirectX::XMFLOAT4& color);
    void SetSpecular4f(const DirectX::XMFLOAT4& color);

protected:
    virtual bool VDelegateInit(TiXmlElement* pData) override;
    virtual std::shared_ptr<SceneNode> VCreateSceneNode() override;
    virtual void VCreateInheritedXmlElements(TiXmlElement* pBaseElement) override;
};