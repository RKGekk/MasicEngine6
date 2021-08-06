#include "smoke_component.h"

#include <DirectXMath.h>

#include "../engine/engine.h"
#include "../engine/d3d_renderer11.h"
#include "transform_component.h"
#include "../events/evt_data_new_render_component.h"
#include "../events/i_event_manager.h"
#include "../tools/memory_utility.h"
#include "../tools/math_utitity.h"
#include "../physics/particle_gravity.h"

const std::string SmokeComponent::g_Name = "SmokeComponent"s;

const std::string& SmokeComponent::VGetName() const {
    return SmokeComponent::g_Name;
}

SmokeComponent::SmokeComponent() : m_particle_world(1024, 16) {
    m_total_time = 0.0f;
    m_smoke_atlas_width = 1;
    m_smoke_atlas_height = 1;
    m_atlas_name = "NoAtlas";
    m_radius = 1.0f;
    m_pixelShaderResource = "NoPShader";
    m_vertexShaderResource = "NoVShader";
}

SmokeComponent::~SmokeComponent() {}

TiXmlElement* SmokeComponent::VGenerateXml() {
    return nullptr;
}

void SmokeComponent::VPostInit() {
    using namespace DirectX;
    std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
    m_force_generators.push_back(std::make_shared<ParticleGravity>(DirectX::XMVectorScale(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), 1.8f)));
    int particle_count = 16;
    m_particles.reserve(particle_count);
    MTRandom& rnd = g_pApp->GetGameLogic()->GetRNG();
    for (int i = 0; i < particle_count; ++i) {
        std::shared_ptr<Particle> particle = std::make_shared<Particle>();
        //particle->setPosition(pTransformComponent->GetPosition());
        particle->setPosition(pTransformComponent->GetPosition() + XMVectorSet(0.0f, m_radius * 0.25f, 0.0f, 0.0f));
        //particle->setPosition(0.0f, 0.0f, 0.0f);
        float vx = ((2.0f * rnd.Random()) - 1.0f) * 0.2f; float vy = ((2.0f * rnd.Random()) - 1.0f) * 0.2f; float vz = ((2.0f * rnd.Random()) - 1.0f) * 0.2f;
        particle->setVelocity(vx, vy, vz);
        particle->setAcceleration(0.0f, 0.0f, 0.0f);
        particle->setMass(1.0f);
        particle->setDamping(0.9f);
        particle->setRadius(m_radius);
        //float ttl = 1.0f + rnd.Random() * 2.0f;
        float ttl = rnd.Random();
        particle->setTTL(ttl);
        particle->setAwake(false);
        particle->setCanSleep(false);

        m_particles.push_back(particle);
        m_particle_world.getParticles().push_back(particle.get());
        for (auto& fg : m_force_generators) {
            m_particle_world.getForceRegistry().add(particle.get(), fg.get());
        }
    }

    BaseRenderComponent::VPostInit();
}

void SmokeComponent::VUpdate(float deltaMs) {
    using namespace DirectX;
    std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
    MTRandom& rnd = g_pApp->GetGameLogic()->GetRNG();

    m_total_time += deltaMs;
    m_particle_world.startFrame();
    for (std::shared_ptr<Particle>& p : m_particles) {
        float ttl_normal = p->getTTLNormal();
        if (DirectX::XMScalarNearEqual(ttl_normal, 0.0f, EPSILON * 100.0f)) {
            //p->setPosition(pTransformComponent->GetPosition());
            p->setPosition(pTransformComponent->GetPosition() + XMVectorSet(0.0f, m_radius * 0.25f, 0.0f, 0.0f));
            float vx = rnd.Random() * 0.3f; float vy = rnd.Random() * 0.3f; float vz = rnd.Random() * 0.3f;
            p->setVelocity(vx, 0.0f, vz);
            p->setAcceleration(0.0f, 0.0f, 0.0f);
            //float ttl = 1.0f + rnd.Random() * 2.0f;
            float ttl = rnd.Random();
            p->setTTL(ttl);
            p->clearTimeAccumulator();
        }
    }
    m_particle_world.runPhysics(deltaMs);
}

std::vector<SmokePointSprite> SmokeComponent::GetMesh() {
    std::vector<SmokePointSprite> res;
    res.reserve(m_particle_world.getParticles().size());
    for (const Particle* p : m_particle_world.getParticles()) {
        res.push_back({ p->getPosition3f(), {m_radius, m_radius}, p->getTTLNormal() });
    }
    return res;
}

const std::string& SmokeComponent::GetPixelShaderResourece() {
    return m_pixelShaderResource;
}

const std::string& SmokeComponent::GetVertexShaderResourece() {
    return m_vertexShaderResource;
}

const std::string& SmokeComponent::GetGeometryShaderResourece() {
    return m_geometryShaderResource;
}

const std::vector<D3D11_INPUT_ELEMENT_DESC>& SmokeComponent::GetVertexShaderInputLayout() {
    return m_vs_layout;
}

const std::string& SmokeComponent::GetTexture() {
    return m_atlas_name;
}

int SmokeComponent::GetAtlasWidth() {
    return m_smoke_atlas_width;
}

int SmokeComponent::GetAtlasHeight() {
    return m_smoke_atlas_height;
}

bool SmokeComponent::VDelegateInit(TiXmlElement* pData) {
    TiXmlElement* pAtlasName = pData->FirstChildElement("AtlasName");
    if (pAtlasName) {
        m_atlas_name = pAtlasName->FirstChild()->Value();
    }

    TiXmlElement* pAtlasWidth = pData->FirstChildElement("AtlasWidth");
    if (pAtlasWidth) {
        std::string width_str = pAtlasWidth->FirstChild()->Value();
        int with = std::stoi(width_str);
        m_smoke_atlas_width = with;
    }

    TiXmlElement* pAtlasHeight = pData->FirstChildElement("AtlasHeight");
    if (pAtlasHeight) {
        std::string height_str = pAtlasHeight->FirstChild()->Value();
        int height = std::stoi(height_str);
        m_smoke_atlas_height = height;
    }

    TiXmlElement* pRadius = pData->FirstChildElement("Radius");
    if (pRadius) {
        std::string radius_str = pRadius->FirstChild()->Value();
        float radius = std::stof(radius_str);
        m_radius = radius;
    }

    TiXmlElement* pPixelShader = pData->FirstChildElement("PixelShader");
    if (pPixelShader) {
        m_pixelShaderResource = pPixelShader->FirstChild()->Value();
    }

    TiXmlElement* pVertexShader = pData->FirstChildElement("VertexShader");
    if (pVertexShader) {
        m_vertexShaderResource = pVertexShader->FirstChild()->Value();
    }

    TiXmlElement* pGeometryShader = pData->FirstChildElement("GeometryShader");
    if (pGeometryShader) {
        m_geometryShaderResource = pGeometryShader->FirstChild()->Value();
    }

    m_vs_layout = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TTL", 0, DXGI_FORMAT_R32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    return true;
}

std::shared_ptr<SceneNode> SmokeComponent::VCreateSceneNode() {
    std::shared_ptr<TransformComponent> pTransformComponent = MakeStrongPtr(m_pOwner->GetComponent<TransformComponent>(TransformComponent::g_Name));
    std::shared_ptr<SceneNode> root_node(new D3D11GeometryPipeline(*this, &pTransformComponent->GetTransform4x4f()));
    root_node->SetAlpha(GetColor().w);
    return root_node;
}

void SmokeComponent::VCreateInheritedXmlElements(TiXmlElement* pBaseElement) {}
