// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeData.h"

const FVector2I MazeData::Directions[DirectionCount] = 
{ 
	// 북, 동, 남, 서 순서
	FVector2I(0, 1), FVector2I(1, 0), FVector2I(0, -1), FVector2I(-1, 0) 
};

MazeData::~MazeData()
{
	// 소멸할 때 메모리 해제
	ClearMaze();
}

void MazeData::MakeMaze(int8 InWidth, int8 InHeight, int32 Seed)
{
	// 크기 저장
	Width = InWidth;
	Height = InHeight;

	// 랜덤 시드 설정
	if (Seed == -1)
	{
		RandomStream.GenerateNewSeed();		
	}
	else
	{
		RandomStream.Initialize(Seed);
	}
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, 
	//	FString::Printf(TEXT("Seed : %d"), RandomStream.GetInitialSeed()));

	// 기존 미로 데이터 삭제
	ClearMaze();

	// 셀 데이터 생성
	Cells = MakeUnique<CellData[]>(Width * Height);	

	// 미로 생성 알고리즘 실행
	AlgorithmExecute();
}

void MazeData::AlgorithmExecute()
{
	// 미로 생성 알고리즘 실행(Wilson 알고리즘)
	TArray<CellData*> UnvisitedCells;			// 방문하지 않은 셀들을 저장할 배열
	UnvisitedCells.Reserve(Width * Height);		// 배열 크기 미리 예약(캐퍼시티 설정)
	for (int y = 0; y < Height; y++)
	{
		for (int x = 0; x < Width; x++)
		{
			// 셀 데이터 초기화
			CellData* Cell = GetCell(x, y);
			Cell->SetLocation(x, y);
			UnvisitedCells.Add(Cell);
		}
	}
	ShuffleArray(UnvisitedCells);				// 셀들을 랜덤하게 섞음

	// 윌슨 알고리즘 시작
	CellData* InitCell = UnvisitedCells.Last();		// 방문하지 않은 셀 중 배열의 마지막 셀을 초기셀으로 설정
	InitCell->SetMazeMember(true);					// 초기셀을 미로에 포함시킴	
	UnvisitedCells.Pop();							// 초기셀을 배열에서 제거

	while (UnvisitedCells.Num() > 0)				
	{
		// 미로에 포함되지 않은 셀 중에서 하나를 랜덤으로 선택
		CellData* StartCell = UnvisitedCells.Last();	// 방문하지 않은 셀 중 배열의 마지막 셀을 시작점으로 설정
		UnvisitedCells.Pop();							// 시작점을 배열에서 제거

		CellData* CurrentCell = StartCell;				// 현재 셀을 시작점으로 설정

		// CurrentCell 위치에서 랜덤으로 한칸 이동
		do
		{
			CellData* NeighborCell = GetRandomNeighborCell(*CurrentCell);	// 현재 셀의 이웃셀 중 랜덤하게 선택
			CurrentCell->SetNextCell(NeighborCell);							// 현재 셀의 다음 셀을 이웃셀로 설정
			CurrentCell = NeighborCell;										// 현재 셀을 이웃셀로 변경
		} while (!CurrentCell->IsMazeMember());			// 현재 셀이 미로에 포함될 때까지 반복

		// 시작점에서 현재 셀까지의 경로를 연결
		CellData* PathCell = StartCell;
		while (PathCell != CurrentCell)
		{
			PathCell->SetMazeMember(true);				// 경로에 있는 셀들을 미로에 포함시킴
			UnvisitedCells.Remove(PathCell);			// 경로에 있는 셀들을 방문하지 않은 셀 배열에서 제거
			ConnectCells(PathCell, PathCell->GetNextCell());	// 경로에 있는 셀과 그 다음 셀을 연결
			PathCell = PathCell->GetNextCell();			// 다음 셀로 이동
		}

	}// 모든 셀이 미로에 포함될 때까지 반복
}

void MazeData::ConnectCells(CellData* InFrom, CellData* InTo)
{
	if (InFrom != nullptr && InTo != nullptr)	// 두 셀이 유효해야만 연결 실행
	{
		if (InFrom->GetY() < InTo->GetY())			// From이 To보다 북쪽에 있을 때
		{
			InFrom->MakePath(EDirectionType::South);
			InTo->MakePath(EDirectionType::North);
		}
		else if (InFrom->GetY() > InTo->GetY())		// From이 To보다 남쪽에 있을 때
		{
			InFrom->MakePath(EDirectionType::North);
			InTo->MakePath(EDirectionType::South);
		}
		else if (InFrom->GetX() < InTo->GetX())		// From이 To보다 동쪽에 있을 때
		{
			InFrom->MakePath(EDirectionType::East);
			InTo->MakePath(EDirectionType::West);
		}
		else if (InFrom->GetX() > InTo->GetX())		// From이 To보다 서쪽에 있을 때
		{
			InFrom->MakePath(EDirectionType::West);
			InTo->MakePath(EDirectionType::East);
		}
	}
}

CellData* MazeData::GetRandomNeighborCell(const CellData& InCell) const
{
	// 랜덤하게 이웃셀을 선택
	FVector2I NeighborLocation;

	do
	{
		int Index = RandomStream.RandRange(0, DirectionCount - 1);		// 0~3 사이의 랜덤한 인덱스 선택
		NeighborLocation = InCell.GetLocation() + Directions[Index];	// 선택된 방향으로 이동
	} while (!IsValidLocation(NeighborLocation.X, NeighborLocation.Y));	// 이동한 위치가 유효하지 않으면 반복

	return GetCell(NeighborLocation.X, NeighborLocation.Y); // 이동한 위치의 셀 반환
}

void MazeData::ShuffleArray(TArray<CellData*>& InArray)
{
	// 피셔 예이츠 알고리즘
	for (int i = InArray.Num() - 1; i > 0; i--)
	{
		int Index = RandomStream.RandRange(0, i);
		InArray.Swap(i, Index);
	}
}
