#pragma once
#include "CellBase.h"

class Cell_Eller : public CellBase
{
public:
	inline int GetSetGroup() const { return SetGrounp; }
	inline void SetSetGroup(int SetGroup) { SetGrounp = SetGroup; }

private:
	// 세트가 설정 안되었을 때의 값
	static const int NotSet = -1;

	// 세트 번호
	int SetGrounp = NotSet;

};

