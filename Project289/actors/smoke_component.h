#pragma once

#include <string>
#include <memory>
#include <vector>

#include <d3d11.h>
#include <DirectXMath.h>

#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

#include "actor_component.h"
#include "base_render_component.h"
#include "../physics/particle.h"
#include "../physics/particle_world.h"
#include "../physics/force_generator.h"
#include "../graphics/shader.h"
#include "../graphics/material_texture.h"
#include "../graphics/vertex.h"
#include "../nodes/d3d_11_geometry_pipeline.h"

using namespace std::literals;

class SmokeComponent : public BaseRenderComponent {
public:
    static const std::string g_Name;
    virtual const std::string& VGetName() const override;

public:
    SmokeComponent();
    virtual ~SmokeComponent();

    virtual TiXmlElement* VGenerateXml() override;
    virtual void VPostInit() override;
    virtual void VUpdate(float deltaMs) override;

    std::vector<SmokePointSprite> GetMesh();
    const std::string& GetPixelShaderResourece();
    const std::string& GetVertexShaderResourece();
    const std::string& GetGeometryShaderResourece();
    const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetVertexShaderInputLayout();
    const std::string& GetTexture();
    int GetAtlasWidth();
    int GetAtlasHeight();
    
protected:
    virtual bool VDelegateInit(TiXmlElement* pData) override;
    virtual std::shared_ptr<SceneNode> VCreateSceneNode() override;
    virtual void VCreateInheritedXmlElements(TiXmlElement* pBaseElement) override;

protected:
    ParticleWorld m_particle_world;
    std::vector<std::shared_ptr<Particle>> m_particles;
    std::vector<std::shared_ptr<ParticleForceGenerator>> m_force_generators;
    int m_smoke_atlas_width;
    int m_smoke_atlas_height;
    std::string m_atlas_name;
    float m_total_time;
    float m_radius;
    std::vector<SmokePointSprite> m_mesh;
    std::string m_pixelShaderResource;
    std::string m_vertexShaderResource;
    std::string m_geometryShaderResource;
    std::vector<D3D11_INPUT_ELEMENT_DESC> m_vs_layout;
};