#pragma once

#include <d3d11.h>
#include <wrl.h>

enum class BindableType {
	blend_state,
	depth_stencil_state,
	index_buffer,
	input_layout,
	pixel_constant_buffer,
	pixel_shader,
	rasterizer_state,
	sampler_state,
	shader_resource,
	topology,
	vertex_buffer_bindable,
	vertex_constant_buffer,
	vertex_shader
};

class Bindable {
public:
	virtual void Bind(ID3D11DeviceContext* deviceContext) = 0;
	virtual ~Bindable() = default;
};