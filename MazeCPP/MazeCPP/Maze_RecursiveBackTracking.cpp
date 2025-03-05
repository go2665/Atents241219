#include "Maze_RecursiveBackTracking.h"
#include "Cell_RecursiveBackTracking.h"
#include "Random.h"

void Maze_RecursiveBackTracking::OnSpecificAlgorithmExcute()
{
    // 재귀적 백트래킹 알고리즘(Recursive BackTracking Algorithm)
    // 1. 미로에서 랜덤한 지점을 방문했다고 표시한다.
    // 2. 마지막에 미로에 추가한 지점에서 갈 수 있는 방향(이전에 방문하지 않은 곳) 중 하나를 선택해서 랜덤하게 이동한다.
    // 3. 이동한 곳은 방문했다고 표시되고, 이전 지점과의 통로가 연결된다. 그리고 다시 2번 수행하기(이동 할 곳이 없을때까지)
    // 4. 이동 할 곳이 없을 경우 이전 단계의 셀로 돌아간다. 그리고 다시 2번 수행하기
    // 5. 시작지점까지 돌아가면 알고리즘 종료

    // 셀 준비하기
    int Size = Width * Height;	// 전체 셀의 갯수
    Cell_RecursiveBackTracking* BackTrackingCells = new Cell_RecursiveBackTracking[Size]();	// 재귀적 백트래킹 알고리즘을 위한 셀 배열 생성
    for (int i = 0; i < Size; ++i)
    {
        Cells[i] = &BackTrackingCells[i];			// CellBase* 배열에 Cell_RecursiveBackTracking의 주소를 저장
		Cells[i]->SetLocation(i % Width, i / Width);	// 셀의 위치를 설정
    }

	// 1. 미로에서 랜덤한 지점을 방문했다고 표시한다.
	int Index = Random::Get().GetRandomInRange(0, Size - 1);	// 랜덤한 인덱스를 선택
	Cell_RecursiveBackTracking* StartCell = static_cast<Cell_RecursiveBackTracking*>(Cells[Index]);	// 선택한 인덱스의 셀을 시작 셀로 설정
	StartCell->SetVisited(true);	// 시작 셀을 방문했다고 표시

    // 재귀문 시작
	MakeRecursive(StartCell->GetX(), StartCell->GetY());
}

void Maze_RecursiveBackTracking::MakeRecursive(int X, int Y)
{
	Cell_RecursiveBackTracking* CurrentCell = static_cast<Cell_RecursiveBackTracking*>(GetCell(X, Y));	// 현재 셀을 가져온다.

	FVector2I Directions[4] = { FVector2I(0, 1), FVector2I(1, 0), FVector2I(0, -1), FVector2I(-1, 0) };	// 4방향을 설정
	Suffle(Directions, 4);	// 4방향을 무작위로 섞는다.

    for (FVector2I Dir : Directions)
    {
		FVector2I NewPosition = FVector2I(X, Y) + Dir;	// 이동할 새로운 위치

        if (IsValidLocation(NewPosition.X, NewPosition.Y))	// 미로 내부인 위치라면
        {
			Cell_RecursiveBackTracking* Neighbor = 
                static_cast<Cell_RecursiveBackTracking*>(GetCell(NewPosition.X, NewPosition.Y));	// 이동할 후보셀을 가져온다.

			if (!Neighbor->IsVisited())	// 방문하지 않은 곳이라면
            {
				Neighbor->SetVisited(true);	        // 방문했다고 표시
				ConnectPath(CurrentCell, Neighbor);	// 현재 셀과 이동할 셀을 연결한다.

				MakeRecursive(NewPosition.X, NewPosition.Y);	// Neighbor를 기준으로 재귀문 실행
            }
        }
    }	
}

void Maze_RecursiveBackTracking::Suffle(FVector2I* Directions, int Count)
{
    // 피셔 예이츠 알고리즘
    for (int i = Count - 1; i > 0; --i)     
    {
        int Index = Random::Get().GetRandomInRange(0, i);   // 랜덤하게 하나 골라서
		std::swap(Directions[i], Directions[Index]);		// 현재 마지막 위치와 바꾼다.
    }
}


