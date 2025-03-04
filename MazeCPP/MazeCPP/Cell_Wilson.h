#pragma once
#include "CellBase.h"

class Cell_Wilson : public CellBase
{
public:
	// getter
	inline Cell_Wilson* GetNextCell() const { return NextCell; }
	inline bool IsMazeMember() const { return bIsMazeMember; }

	// setter
	inline void SetNextCell(Cell_Wilson* NextCell) { this->NextCell = NextCell; }
	inline void SetMazeMember(bool bIsMazeMember) { this->bIsMazeMember = bIsMazeMember; }

private:
	// 다음 셀(길을 뚫어야 하는 방향에 있는 셀)
	Cell_Wilson* NextCell = nullptr;	

	// 미로에 포함되었는지 여부(true면 포함, false면 미포함)
	bool bIsMazeMember = false;
};