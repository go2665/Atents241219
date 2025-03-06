// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeActor.h"
#include "MazeData.h"
#include "CellActor.h"

// Sets default values
AMazeActor::AMazeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AMazeActor::BeginPlay()
{
	Super::BeginPlay();

	// 미로 데이터 생성
	MazeData* Maze = new MazeData();
	Maze->MakeMaze(Width, Height, RandomSeed);	// 미로 데이터 완료

	// 미로 데이터를 이용하여 셀 생성
	if (CellActorClass)
	{
		float CellHalfSize = 1000.0f;
		float CellSize = CellHalfSize * 2.0f;
		FVector StartLocation = FVector(Height * CellHalfSize, -Width * CellHalfSize , 0.0f)
			 + FVector(-CellHalfSize * (Height % 2), CellHalfSize * (Width % 2), 0);	// 미로의 시작 위치
		UWorld* World = GetWorld();

		for (int y = 0; y < Height; y++)
		{
			for (int x = 0; x < Width; x++)
			{
				CellData* Cell = Maze->GetCell(x, y);	// 셀을 가져와서
				if (Cell)
				{
					FVector Location = StartLocation + FVector(-y * CellSize, x * CellSize, 0.0f);	// 셀의 위치 계산

					ACellActor* CellActor = World->SpawnActor<ACellActor>(	// 셀 엑터 스폰
						CellActorClass, 
						Location, 
						FRotator::ZeroRotator);
					if (CellActor)
					{
						CellActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);	// 미로 액터에 붙임
						CellActor->Initialize(Cell);	// 셀 데이터 초기화
					}
				}
			}
		}
	}

	// 미로 데이터 정리
	Maze->ClearMaze();
	delete Maze;
	Maze = nullptr;
	
}
