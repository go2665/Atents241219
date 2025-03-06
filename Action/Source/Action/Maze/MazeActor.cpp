// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeActor.h"
#include "MazeData.h"

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
		FVector StartLocation = FVector(-Height * CellHalfSize, -Width * CellHalfSize , 0.0f);


		for (int y = 0; y < Height; y++)
		{
			for (int x = 0; x < Width; x++)
			{
				CellData* Cell = Maze->GetCell(x, y);
				if (Cell)
				{
					FVector Location = FVector(-x * CellSize, y * CellSize, 0.0f);



					//ACellActor* CellActor = GetWorld()->SpawnActor<ACellActor>(CellActorClass, Location, FRotator::ZeroRotator);
					//if (CellActor)
					//{
					//	CellActor->Initialize(Cell);
					//}
				}
			}
		}
	}


	// 미로 데이터 정리
	Maze->ClearMaze();
	delete Maze;
	Maze = nullptr;
	
}
