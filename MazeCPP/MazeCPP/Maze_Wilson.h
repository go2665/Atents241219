#pragma once
#include "MazeBase.h"
#include "Cell_Wilson.h"
#include "FVector2I.h"

class Maze_Wilson :public MazeBase
{
public:
	virtual ~Maze_Wilson() override = default;	// MazeBase의 소멸자는 자동 실행

protected:
	// 윌슨 알고리즘을 실행하는 함수
	virtual void OnSpecificAlgorithmExcute() override;

private:
	// 이웃 셀 중 랜덤하게 하나를 리턴하는 함수
	Cell_Wilson* GetRandomNeighbor(const Cell_Wilson* Cell) const;

	// 방향 갯수(북동남서)
	static const int DirectionsCount = 4;

	static const FVector2I Directions[DirectionsCount];
};