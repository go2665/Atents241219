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
	if (Seed != -1)
	{
		RandomStream.Initialize(Seed);
	}

	// 기존 미로 데이터 삭제
	ClearMaze();

	// 셀 데이터 생성
	Cells = new CellData[Width * Height];

	// 미로 생성 알고리즘 실행
	AlgorithmExecute();
}

void MazeData::AlgorithmExecute()
{

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
