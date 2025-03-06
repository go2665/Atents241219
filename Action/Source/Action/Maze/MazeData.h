// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CellData.h"
#include "FVector2I.h"

/**
 * 미로를 생성하고 그 데이터를 가지고 있는 클래스
 */
class ACTION_API MazeData
{
public:
	// 가상 소멸자
	virtual ~MazeData();

	// 미로를 생성하는 함수
	void MakeMaze(int8 InWidth, int8 InHeight, int32 Seed = -1);

	// 생성된 미로를 제거하는 함수
	inline void ClearMaze() 
	{
		Cells.Reset();
	}

	// 위치에 해당하는 셀을 반환하는 함수
	inline CellData* GetCell(int8 InX, int8 InY) const
	{
		if (!IsValidLocation(InX, InY)) return nullptr;
		return &Cells[LocationToIndex(InX, InY)];
	}

private:
	// 미로 생성 알고리즘을 실행하는 함수
	void AlgorithmExecute();

	// 미로의 셀을 연결하는 함수
	void ConnectCells(CellData* InFrom, CellData* InTo);

	// 미로의 이웃셀 중 랜덤하게 하나를 골라 반환하는 함수
	CellData* GetRandomNeighborCell(const CellData& InCell) const;

	// 위치를 인덱스로 변환하는 함수
	inline int16 LocationToIndex(int8 InX, int8 InY) const { return InY * Width + InX; }

	// 위치가 유효한지 확인하는 함수
	inline bool IsValidLocation(int8 InX, int8 InY) const { return InX >= 0 && InX < Width && InY >= 0 && InY < Height; }

	// 배열을 랜덤하게 섞는 함수
	void ShuffleArray(TArray<CellData*>& InArray);

private:
	// 미로의 너비
	int8 Width = 0;

	// 미로의 높이
	int8 Height = 0;

	// 미로의 셀 데이터의 배열
	TUniquePtr<CellData[]> Cells;

	// 이웃 방향 개수
	static const int DirectionCount = 4;

	// 이웃 방향
	static const FVector2I Directions[DirectionCount];

	// 이 클래스에서 사용할 랜덤 스트림
	FRandomStream RandomStream;
};
