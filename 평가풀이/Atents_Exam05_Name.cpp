#include "Atents_Exam05_Name.h"

void Atents_Exam05_Name::OnMouseUp(WPARAM btnState, int x, int y)
{
	if (bIsLButtonDown && !(btnState & MK_LBUTTON))
	{
		bIsRotate = !bIsRotate;
		bIsLButtonDown = false;
	}
}

void Atents_Exam05_Name::OnMouseDown(WPARAM btnState, int x, int y)
{
	if (btnState & MK_LBUTTON)
	{
		bIsLButtonDown = true;
	}
}

void Atents_Exam05_Name::MakevertecesAndIndeces(std::array<Vertex, 8>* verteces, std::array<std::uint16_t, 12>* indices)
{
	*verteces =
	{
		Vertex({ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT4(Colors::Green) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, 0.0f), XMFLOAT4(Colors::Green) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, 0.0f), XMFLOAT4(Colors::Green) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, 0.0f), XMFLOAT4(Colors::Green) }),

		Vertex({ XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT4(Colors::Orange) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, 0.0f), XMFLOAT4(Colors::Orange) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, 0.0f), XMFLOAT4(Colors::Orange) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, 0.0f), XMFLOAT4(Colors::Orange) })
	};

	*indices =
	{
		0, 1, 2,
		0, 2, 3,
		4, 6, 5,
		4, 7, 6
	};
}

XMMATRIX Atents_Exam05_Name::UpdateWorldMatrix(const GameTimer& gt)
{
	static float angle = 0.0f;

	if (bIsRotate)
	{
		angle += gt.DeltaTime() * XM_2PI;
	}

	return XMMatrixRotationY(angle);
}
