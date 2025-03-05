#pragma once
#include "MazeBase.h"
#include "FVector2I.h"

class Maze_RecursiveBackTracking : public MazeBase
{
public:
	virtual ~Maze_RecursiveBackTracking() override = default;

protected:
	virtual void OnSpecificAlgorithmExcute() override;

private:
	void MakeRecursive(int X, int Y);

	void Suffle(FVector2I* Directions, int Count);
};

