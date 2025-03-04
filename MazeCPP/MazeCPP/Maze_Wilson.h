#pragma once
#include "MazeBase.h"
#include "Cell_Wilson.h"

class Maze_Wilson :public MazeBase
{
public:
	virtual ~Maze_Wilson() override = default;	// MazeBase의 소멸자는 자동 실행

protected:
	virtual void OnSpecificAlgorithmExcute() override;

private:
	Cell_Wilson* GetRandomNeighbor(const Cell_Wilson* Cell) const;
};

