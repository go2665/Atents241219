#include "Chap7App.h"

Chap7App::Chap7App(HINSTANCE hInstance) : D3DApp(hInstance)
{
}

Chap7App::~Chap7App()
{
}

bool Chap7App::Initialize()
{
	return false;
}

void Chap7App::OnResize()
{
}

void Chap7App::Update(const GameTimer& gt)
{
}

void Chap7App::Draw(const GameTimer& gt)
{
}

void Chap7App::OnMouseDown(WPARAM btnState, int x, int y)
{
}

void Chap7App::OnMouseUp(WPARAM btnState, int x, int y)
{
}

void Chap7App::OnMouseMove(WPARAM btnState, int x, int y)
{
}

void Chap7App::OnKeyboardInput(const GameTimer& gt)
{
}

void Chap7App::UpdateCamera9(const GameTimer& gt)
{
}

void Chap7App::UpdateObjectCBs(const GameTimer& gt)
{
}

void Chap7App::UpdateMainPassCB(const GameTimer& gt)
{
}

void Chap7App::BuildDescriptorHeaps()
{
}

void Chap7App::VuildConstantBufferViews()
{
}

void Chap7App::BuildRootSignature()
{
}

void Chap7App::BuildShadersAndInputLayout()
{
}

void Chap7App::BuildShapeGeometry()
{
}

void Chap7App::BuildPSOs()
{
}

void Chap7App::BuildFrameResources()
{
}

void Chap7App::BuildRenderItems()
{
}

void Chap7App::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItemApp7*>& ritems)
{
}
