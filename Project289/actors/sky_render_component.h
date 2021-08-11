#pragma once

#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <unordered_map>

#include <DirectXMath.h>

#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

#include "base_render_component.h"
#include "../bindable/vertex_constant_buffer_bindable.h"
#include "../bindable/pixel_constant_buffer_bindable.h"
#include "../graphics/material_texture.h"
#include "../graphics/constant_buffer_types.h"
#include "../graphics/vertex.h"
#include "../graphics/mesh_holder.h"

class SkyRenderComponent : public BaseRenderComponent {
    std::string m_pixelShaderResource;
    std::string m_vertexShaderResource;
    std::string m_resource_directory;
    std::string m_sky_cube_map_path;

    std::vector<D3D11_INPUT_ELEMENT_DESC> m_vs_layout;
    float m_sky_sphere_radius;
    int m_sky_sphere_slice_count;
    int m_sky_sphere_stack_count;

    MeshHolder m_mesh;

public:
    static const std::string g_Name;
    virtual const std::string& VGetName() const;

    SkyRenderComponent();

    const std::string& GetPixelShaderResource();
    const std::string& GetVertexShaderResource();
    const std::vector<D3D11_INPUT_ELEMENT_DESC>& GetLayout();
    MeshHolder& GetMesh();

protected:
    virtual bool VDelegateInit(TiXmlElement* pData) override;
    virtual std::shared_ptr<SceneNode> VCreateSceneNode() override;

    virtual void VCreateInheritedXmlElements(TiXmlElement* pBaseElement) override;    
};