#include "camera_node.h"
#include "scene.h"
#include "../engine/engine.h"

CameraNode::CameraNode(const DirectX::XMFLOAT4X4& t, const Frustum& frustum) :
	SceneNode(WeakBaseRenderComponentPtr(), RenderPass::RenderPass_0, &t),
	m_Frustum(frustum),
	m_bActive(true),
	m_DebugCamera(false),
	m_pTarget(std::shared_ptr<SceneNode>()),
	m_CamOffsetVector(0.0f, 1.0f, -10.0f, 0.0f)
{
	DirectX::XMStoreFloat4x4(&m_Projection, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_View, DirectX::XMMatrixIdentity());
}

CameraNode::CameraNode(DirectX::FXMMATRIX t, const Frustum& frustum) :
	SceneNode(WeakBaseRenderComponentPtr(), RenderPass::RenderPass_0, t, DirectX::XMMatrixIdentity(), true),
	m_Frustum(frustum),
	m_bActive(true),
	m_DebugCamera(false),
	m_pTarget(std::shared_ptr<SceneNode>()),
	m_CamOffsetVector(0.0f, 1.0f, -10.0f, 0.0f)
{
	DirectX::XMStoreFloat4x4(&m_Projection, DirectX::XMMatrixIdentity());
	DirectX::XMStoreFloat4x4(&m_View, DirectX::XMMatrixIdentity());
}

HRESULT CameraNode::VRender(Scene* pScene) {
	if (m_DebugCamera) 	{
		pScene->PopMatrix();
		m_Frustum.Render();
		pScene->PushAndSetMatrix4x4(m_Props.ToWorld4x4());
	}

	return S_OK;
}

HRESULT CameraNode::VOnRestore(Scene* pScene) {
	m_Frustum.SetAspect(((float)g_pApp->GetConfig().m_screenWidth) / ((float)g_pApp->GetConfig().m_screenHeight));
	DirectX::XMStoreFloat4x4(&m_Projection, DirectX::XMMatrixPerspectiveFovLH(m_Frustum.m_Fov, m_Frustum.m_Aspect, m_Frustum.m_Near, m_Frustum.m_Far));
	pScene->GetRenderer()->VSetProjectionTransform4x4(m_Projection);
	return S_OK;
}

bool CameraNode::VIsVisible(Scene* pScene) const {
	return m_bActive;
}

const Frustum& CameraNode::GetFrustum() {
	return m_Frustum;
}

void CameraNode::SetTarget(std::shared_ptr<SceneNode> pTarget) {
	m_pTarget = pTarget;
}

void CameraNode::ClearTarget() {
	m_pTarget = std::shared_ptr<SceneNode>();
}

std::shared_ptr<SceneNode> CameraNode::GetTarget() {
	return m_pTarget;
}

DirectX::XMMATRIX CameraNode::GetWorldViewProjection(Scene* pScene) {
	DirectX::XMMATRIX world = pScene->GetTopMatrix();
	DirectX::XMMATRIX view = VGet().FromWorld();
	DirectX::XMMATRIX worldView = DirectX::XMMatrixMultiply(world, view);
	return DirectX::XMMatrixMultiply(worldView, DirectX::XMLoadFloat4x4(&m_Projection));
}

DirectX::XMFLOAT4X4 CameraNode::GetWorldViewProjection4x4(Scene* pScene) {
	DirectX::XMFLOAT4X4 res;
	DirectX::XMStoreFloat4x4(&res, GetWorldViewProjection(pScene));
	return res;
}

DirectX::XMFLOAT4X4 CameraNode::GetWorldViewProjection4x4T(Scene* pScene) {
	DirectX::XMFLOAT4X4 res;
	DirectX::XMStoreFloat4x4(&res, DirectX::XMMatrixTranspose(GetWorldViewProjection(pScene)));
	return res;
}

DirectX::XMMATRIX CameraNode::GetViewProjection() {
	DirectX::XMMATRIX view = VGet().FromWorld();
	return DirectX::XMMatrixMultiply(view, DirectX::XMLoadFloat4x4(&m_Projection));
}

DirectX::XMFLOAT4X4 CameraNode::GetViewProjection4x4() {
	DirectX::XMFLOAT4X4 res;
	DirectX::XMStoreFloat4x4(&res, GetViewProjection());
	return res;
}

DirectX::XMFLOAT4X4 CameraNode::GetViewProjection4x4T() {
	DirectX::XMFLOAT4X4 res;
	DirectX::XMStoreFloat4x4(&res, DirectX::XMMatrixTranspose(GetViewProjection()));
	return res;
}

HRESULT CameraNode::SetViewTransform(Scene* pScene) {
	using namespace DirectX;
	if (m_pTarget) {
		DirectX::XMMATRIX mat = m_pTarget->VGet().ToWorld();
		DirectX::XMVECTOR at = DirectX::XMLoadFloat4(&m_CamOffsetVector);
		DirectX::XMVECTOR atWorld = DirectX::XMVector4Transform(at, mat);
		mat.r[3] += atWorld;
		VSetTransform(mat, DirectX::XMMatrixIdentity(), true);
	}

	DirectX::XMStoreFloat4x4(&m_View, VGet().FromWorld());
	pScene->GetRenderer()->VSetViewTransform4x4(m_View);

	return S_OK;
}

DirectX::XMMATRIX CameraNode::GetProjection() {
	return DirectX::XMLoadFloat4x4(&m_Projection);
}

const DirectX::XMFLOAT4X4& CameraNode::GetProjection4x4f() {
	return m_Projection;
}

DirectX::XMFLOAT4X4 CameraNode::GetProjection4x4fT() {
	DirectX::XMMATRIX t = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_Projection));
	DirectX::XMFLOAT4X4 t4x4;
	DirectX::XMStoreFloat4x4(&t4x4, t);
	return t4x4;
}


DirectX::XMMATRIX CameraNode::GetView() {
	return DirectX::XMLoadFloat4x4(&m_View);
}

const DirectX::XMFLOAT4X4& CameraNode::GetView4x4() {
	return m_View;
}

DirectX::XMFLOAT4X4 CameraNode::GetView4x4T() {
	DirectX::XMMATRIX t = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(&m_View));
	DirectX::XMFLOAT4X4 t4x4;
	DirectX::XMStoreFloat4x4(&t4x4, t);
	return t4x4;
}

void CameraNode::SetCameraOffset(const DirectX::XMFLOAT4& cameraOffset) {
	m_CamOffsetVector = cameraOffset;
}
