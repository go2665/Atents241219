#pragma once
#include "CellBase.h"
class MazeBase
{
public :
	// 미로의 가상소멸자
	virtual ~MazeBase() { ClearMaze(); }

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

	// 미로의 전체 셀 반환
	inline CellBase** GetCells() const { return Cells; }

protected:
	// 각 알고리즘 별로 미로를 생성하기 위해 실제 알고리즘을 수행하는 함수
	virtual void OnSpecificAlgorithmExcute() = 0;

	// 두 셀사이를 길로 연결하는 함수(from과 to는 반드시 인접해있는 셀이어야 한다.)
	void ConnectPath(CellBase* From, CellBase* To);

	// 특정한 위치를 배열용 인덱스로 변환해주는 함수
	inline int LocationToIndex(int X, int Y) const { return Y * Width + X; }

	// 특정 위치의 셀을 반환하는 함수
	inline CellBase* GetCell(int X, int Y) const 
	{ 
		if (!IsValidLocation(X, Y)) return nullptr;
		return Cells[LocationToIndex(X, Y)]; 
	}

	// 특정 위치의 셀이 있는지 확인하는 함수(셀이 있으면 true, 없으면 false)
	inline bool IsValidLocation(int X, int Y) const { return X >= 0 && X < Width && Y >= 0 && Y < Height; }

protected:
	// 미로의 가로 크기
	int Width = 0;

	// 미로의 세로 크기
	int Height = 0;

	// CellBase를 상속받은 셀들의 주소를 저장하기 위한 포인터 배열
	CellBase** Cells = nullptr;
};

