#pragma once
#include "Atents_Exam05.h"

class Atents_Exam05_Name : public Atents_Exam05
{
public:
	Atents_Exam05_Name(HINSTANCE hInstance) : Atents_Exam05(hInstance) {}
	virtual ~Atents_Exam05_Name() {};
	Atents_Exam05_Name(const Atents_Exam05_Name& rhs) = delete;
	Atents_Exam05_Name& operator=(const Atents_Exam05_Name& rhs) = delete;

protected:
	virtual void OnMouseUp(WPARAM btnState, int x, int y) override;
	virtual void OnMouseDown(WPARAM btnState, int x, int y) override;

	virtual void MakevertecesAndIndeces(std::array<Vertex, 8>* verteces, std::array<std::uint16_t, 12>* indices) override;
	virtual XMMATRIX UpdateWorldMatrix(const GameTimer& gt) override;

private:
	bool bIsRotate = true;	
	bool bIsLButtonDown = false;
};

