#include "light_render_component.h"
#include "../nodes/d3d_light_node_11.h"
#include "../engine/engine.h"

const std::string LightRenderComponent::g_Name = "LightRenderComponent";

const std::string& LightRenderComponent::VGetName() const {
	return g_Name;
}

LightRenderComponent::LightRenderComponent() {}

const LightProperties& LightRenderComponent::GetLight() const {
	return m_Props;
}

void LightRenderComponent::SetAmbient4f(const DirectX::XMFLOAT4& color) {
	m_Props.m_Ambient = color;
}

void LightRenderComponent::SetDiffuse4f(const DirectX::XMFLOAT4& color) {
	m_Props.m_Diffuse = color;
}

void LightRenderComponent::SetSpecular4f(const DirectX::XMFLOAT4& color) {
	m_Props.m_Specular = color;
}

void LightRenderComponent::SetAttenuation(float c, float l, float e) {
	m_Props.m_Attenuation[0] = c;
	m_Props.m_Attenuation[1] = l;
	m_Props.m_Attenuation[2] = e;
}

void LightRenderComponent::SetRange(float r) {
	m_Props.m_Range = r;
}

void LightRenderComponent::SetSpot(float s) {
	m_Props.m_Spot = s;
}

bool LightRenderComponent::VDelegateInit(TiXmlElement* pData) {
	TiXmlElement* pLight = pData->FirstChildElement("Light");

	m_Props.m_LightType = LightType::DIRECTIONAL;
	const char* at = pLight->Attribute("type");
	std::string lightType(at == nullptr ? "" : at);
	if (!lightType.empty()) {
		if (lightType == "DirectionalLight") {
			m_Props.m_LightType = LightType::DIRECTIONAL;
		}
		if (lightType == "PointLight") {
			m_Props.m_LightType = LightType::POINT;
		}
		if (lightType == "SpotLight") {
			m_Props.m_LightType = LightType::SPOT;
		}
	}

	const char* is_lantern_char = pLight->Attribute("lantern");
	std::string is_lantern_string(is_lantern_char == nullptr ? "" : is_lantern_char);
	if (is_lantern_string == "true" || is_lantern_string == "1" || is_lantern_string == "TRUE") {
		m_Props.m_is_lantern = true;
	}
	else {
		m_Props.m_is_lantern = false;
	}

	double temp;
	TiXmlElement* pAttenuationNode = NULL;
	pAttenuationNode = pLight->FirstChildElement("Attenuation");
	if (pAttenuationNode) {
		pAttenuationNode->Attribute("const", &temp);
		m_Props.m_Attenuation[0] = (float)temp;

		pAttenuationNode->Attribute("linear", &temp);
		m_Props.m_Attenuation[1] = (float)temp;

		pAttenuationNode->Attribute("exp", &temp);
		m_Props.m_Attenuation[2] = (float)temp;
	}

	TiXmlElement* pAmbientNode = NULL;
	pAmbientNode = pLight->FirstChildElement("Ambient");
	if (pAmbientNode) {
		pAmbientNode->Attribute("r", &temp);
		m_Props.m_Ambient.x = (float)temp;

		pAmbientNode->Attribute("g", &temp);
		m_Props.m_Ambient.y = (float)temp;

		pAmbientNode->Attribute("b", &temp);
		m_Props.m_Ambient.z = (float)temp;

		pAmbientNode->Attribute("a", &temp);
		m_Props.m_Ambient.w = (float)temp;
	}

	TiXmlElement* pDiffuseNode = NULL;
	pDiffuseNode = pLight->FirstChildElement("Diffuse");
	if (pDiffuseNode) {
		pDiffuseNode->Attribute("r", &temp);
		m_Props.m_Diffuse.x = (float)temp;

		pDiffuseNode->Attribute("g", &temp);
		m_Props.m_Diffuse.y = (float)temp;

		pDiffuseNode->Attribute("b", &temp);
		m_Props.m_Diffuse.z = (float)temp;

		pDiffuseNode->Attribute("a", &temp);
		m_Props.m_Diffuse.w = (float)temp;
	}

	TiXmlElement* pSpecularNode = NULL;
	pSpecularNode = pLight->FirstChildElement("Specular");
	if (pSpecularNode) {
		pSpecularNode->Attribute("r", &temp);
		m_Props.m_Specular.x = (float)temp;

		pSpecularNode->Attribute("g", &temp);
		m_Props.m_Specular.y = (float)temp;

		pSpecularNode->Attribute("b", &temp);
		m_Props.m_Specular.z = (float)temp;

		pSpecularNode->Attribute("a", &temp);
		m_Props.m_Specular.w = (float)temp;
	}

	TiXmlElement* pShapeNode = NULL;
	pShapeNode = pLight->FirstChildElement("Shape");
	if (pShapeNode) {
		pShapeNode->Attribute("range", &temp);
		m_Props.m_Range = (float)temp;
		pShapeNode->Attribute("spot", &temp);
		m_Props.m_Spot = (float)temp;
	}
	return true;
}

std::shared_ptr<SceneNode> LightRenderComponent::VCreateSceneNode() {
	std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
	if (pTransformComponent) {
		BaseRenderComponent* weakThis(this);
		switch (Engine::GetRendererImpl()) {
			case Renderer::Renderer_D3D11: {
				return std::shared_ptr<SceneNode>(new D3DLightNode11(weakThis, m_Props, pTransformComponent->GetTransform(), DirectX::XMMatrixIdentity(), true));
			}
		}
	}
	return std::shared_ptr<SceneNode>();
}

void LightRenderComponent::VCreateInheritedXmlElements(TiXmlElement* pBaseElement) {
	TiXmlElement* pSceneNode = new TiXmlElement("Light");

	TiXmlElement* pAttenuation = new TiXmlElement("Attenuation");
	pAttenuation->SetAttribute("const", std::to_string(m_Props.m_Attenuation[0]).c_str());
	pAttenuation->SetAttribute("linear", std::to_string(m_Props.m_Attenuation[1]).c_str());
	pAttenuation->SetAttribute("exp", std::to_string(m_Props.m_Attenuation[2]).c_str());
	pSceneNode->LinkEndChild(pAttenuation);

	std::unique_ptr<TiXmlElement> pShape = std::make_unique<TiXmlElement>("Shape");
	pShape->SetAttribute("range", std::to_string(m_Props.m_Range).c_str());
	pShape->SetAttribute("falloff", std::to_string(m_Props.m_Spot).c_str());
	pSceneNode->LinkEndChild(pShape.get());

	pBaseElement->LinkEndChild(pSceneNode);
}
