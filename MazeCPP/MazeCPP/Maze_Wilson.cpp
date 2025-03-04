#include "Maze_Wilson.h"

const FVector2I Maze_Wilson::Directions[Maze_Wilson::DirectionsCount] = 
{
	FVector2I(0, 1),	// 북
	FVector2I(1, 0),	// 동
	FVector2I(0, -1),	// 남
	FVector2I(-1, 0)	// 서
};

void Maze_Wilson::OnSpecificAlgorithmExcute()
{
	// Wilson 알고리즘
	// 1. 맵의 한 셀을 랜덤으로 미로에 추가한다.
	// 2. 맵에 있는 미로에 포함되지 않은 셀 중아서 하나를 랜덤으로 선택한다.(StartCell)
	// 3. StartCell의 위치에서 랜덤으로 한 칸 이동한다.(이전 셀에서 이동한 셀을 기록해둬야 한다.)
	// 4. 미로에 포함된 셀에 도착할 때까지 3번을 반복한다
	// 5. StartCell 위치에서 미로에 포함된 영역에 도착할 때까지의 경로에 따라 미로에 포함시킨다.(경로에 따라 벽도 제거)
	// 6. 모든 셀이 미로에 포함될 때까지 2번으로 돌아가 반복한다.
}

Cell_Wilson* Maze_Wilson::GetRandomNeighbor(const Cell_Wilson* Cell) const
{
	return nullptr;
}
