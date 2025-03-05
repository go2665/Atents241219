#include "Maze_Wilson.h"
#include "Random.h"
#include <vector>

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
	// 2. 맵에 있는 미로에 포함되지 않은 셀 중에서 하나를 랜덤으로 선택한다.(StartCell)
	// 3. StartCell의 위치에서 랜덤으로 한 칸 이동한다.(이전 셀에서 이동한 셀을 기록해둬야 한다.)
	// 4. 미로에 포함된 셀에 도착할 때까지 3번을 반복한다
	// 5. StartCell 위치에서 미로에 포함된 영역에 도착할 때까지의 경로에 따라 미로에 포함시킨다.(경로에 따라 벽도 제거)
	// 6. 모든 셀이 미로에 포함될 때까지 2번으로 돌아가 반복한다.

	int Size = Width * Height;	// 전체 셀의 갯수
	Cell_Wilson* WilsonCells = new Cell_Wilson[Size]();	// Wilson 알고리즘을 위한 셀 배열 생성
	for (int i = 0; i < Size; ++i)
	{
		Cells[i] = &WilsonCells[i];			// CellBase* 배열에 Cell_Wilson의 주소를 저장
	}

	std::vector<CellBase*> unvisitedCells;	// 방문하지 않은 모든 셀을 저장하는 벡터
	unvisitedCells.reserve(Size);			// 벡터의 크기를 미리 확보

	// Cells와 unvisitedCells 초기화
	for (int y = 0; y < Height; ++y)
	{
		for (int x = 0; x < Width; ++x)
		{
			CellBase* Cell = GetCell(x, y);		// 셀을 하나 가져온다.
			Cell->SetLocation(x, y);			// 셀의 위치를 설정
			unvisitedCells.push_back(Cell);		// 셀을 방문하지 않은 셀 벡터에 추가
		}
	}

	Random::Get().Shuffle(unvisitedCells);		// unvisitedCells을 무작위로 섞는다.(하나씩 꺼낼 때 랜덤으로 뽑는 것 처럼 보인다.)

	// 1. 맵의 한 셀을 랜덤으로 미로에 추가한다.
	Cell_Wilson* InitCell = static_cast<Cell_Wilson*>(unvisitedCells.back());	// 방문하지 않은 셀 중 하나를 선택
	InitCell->SetMazeMember(true);		// 미로에 포함시킨다.
	unvisitedCells.pop_back();			// InitCell을 방문하지 않은 셀 벡터에서 제거

	while (unvisitedCells.size() > 0)
	{
		// 2. 맵에 있는 미로에 포함되지 않은 셀 중에서 하나를 랜덤으로 선택한다.(StartCell)
		Cell_Wilson* StartCell = static_cast<Cell_Wilson*>(unvisitedCells.back());
		unvisitedCells.pop_back();			// StartCell을 방문하지 않은 셀 벡터에서 제거

		Cell_Wilson* CurrentCell = StartCell;	// 현재 셀을 StartCell로 설정

		// 3. StartCell의 위치에서 랜덤으로 한 칸 이동한다.(이전 셀에서 이동한 셀을 기록해둬야 한다.)
		do
		{
			Cell_Wilson* Neighbor = GetRandomNeighbor(*CurrentCell);	// 이웃 셀 중 하나를 선택
			CurrentCell->SetNextCell(Neighbor);		// 다음 셀로 설정
			CurrentCell = Neighbor;					// 현재 셀을 다음 셀로 설정
		} while (!CurrentCell->IsMazeMember());		// 4. 미로에 포함된 셀에 도착할 때까지 3번을 반복한다

		// 5. StartCell 위치에서 미로에 포함된 영역에 도착할 때까지의 경로에 따라 미로에 포함시킨다.(경로에 따라 벽도 제거)
		Cell_Wilson* PathCell = StartCell;
		while (PathCell != CurrentCell)
		{
			PathCell->SetMazeMember(true);		// 미로에 포함시킨다.
			auto newEnd = std::remove(unvisitedCells.begin(), unvisitedCells.end(), PathCell);	// 방문하지 않은 셀 벡터에서 PathCell을 제거
			unvisitedCells.erase(newEnd, unvisitedCells.end());			// 제거된 부분을 벡터에서 실제로 제거

			ConnectPath(PathCell, PathCell->GetNextCell());	// 길을 연결한다.
			PathCell = PathCell->GetNextCell();				// 다음 셀로 이동
		}
	}// 6. 모든 셀이 미로에 포함될 때까지 2번으로 돌아가 반복한다.
}

Cell_Wilson* Maze_Wilson::GetRandomNeighbor(const Cell_Wilson& Cell) const
{
	FVector2I NeighborLocation;
	do
	{
		int index = Random::Get().GetRandom(Maze_Wilson::DirectionsCount);			// 0~3 사이의 랜덤한 숫자를 얻는다.
		NeighborLocation = FVector2I(Cell.GetX(), Cell.GetY()) + Directions[index];	// 이웃 셀의 위치를 구한다.
	} while (!IsValidLocation(NeighborLocation.X, NeighborLocation.Y));				// 이웃 셀이 유효한 위치가 나올 때까지 반복

	return static_cast<Cell_Wilson*>(GetCell(NeighborLocation.X, NeighborLocation.Y));	// 이웃 셀을 반환
}
