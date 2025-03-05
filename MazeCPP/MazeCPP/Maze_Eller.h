#pragma once
#include "MazeBase.h"
#include "Cell_Eller.h"

class Maze_Eller : public MazeBase
{
public:
	virtual ~Maze_Eller() override = default;

protected:
	virtual void OnSpecificAlgorithmExcute() override;

private:
	// 한 줄을 만드는 함수(위쪽 줄을 참조해서 만든다.)
	Cell_Eller* MakeLine(Cell_Eller* UpperLine);

	// 옆칸끼리 합치는 함수
	void MergeAdjacentCell(Cell_Eller* Line, float Rate);

	// 아래쪽 벽을 제거하는 함수
	void RemoveDownWall(Cell_Eller* Line);

	// 완성된 줄을 Cells에 기록하는 함수
	void WriteLine(Cell_Eller* Line);

	// 옆칸이 다른 집합일 때 합치는 확률
	float AdjacentMergeRate = 0.7f;

	// 아래쪽 벽을 제거하는 확률
	float DownWallRemoveRate = 0.5f;

	// 고유한 집합을 설정하기 위한 시리얼 넘버
	int SerialNumber = 0;
};

