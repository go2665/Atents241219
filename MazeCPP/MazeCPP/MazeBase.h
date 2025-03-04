#pragma once
#include "CellBase.h"
class MazeBase
{
public :
	// 미로의 가상소멸자
	virtual ~MazeBase();

	// 미로 생성 함수
	void MakeMaze(int Width, int Height, int Seed = -1);

	// 미로 제거 함수
	inline void ClearMaze()
	{
		if (Cells != nullptr)
		{
			delete[] Cells[0];	// CellBase를 상속받은 배열 해제
			delete[] Cells;		// CellBase*의 배열 해제
			Cells = nullptr;	// nullptr로 초기화
		}
	}


protected:
	// 미로의 가로 크기
	int Width = 0;

	// 미로의 세로 크기
	int Height = 0;

	// CellBase를 상속받은 셀들의 주소를 저장하기 위한 포인터 배열
	CellBase** Cells = nullptr;
};

