// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeActor.h"
#include "MazeData.h"
#include "CellActor.h"
#include "MazeExitActor.h"

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

	// 시작 위치 및 월드 좌표 계산용 데이터
	UWorld* World = GetWorld();
	float CellHalfSize = 1000.0f;
	float CellSize = CellHalfSize * 2.0f;
	FVector StartLocation = FVector(Height * CellHalfSize, -Width * CellHalfSize, 0.0f)
		+ FVector(-CellHalfSize * (Height % 2), CellHalfSize * (Width % 2), 0);	// 미로의 시작 위치

	// 출구 만들기(위치 확정용)
	int ExitX = 0;
	int ExitY = 0;
	AMazeExitActor* ExitActor = nullptr;
	if (MazeExitActorClass)
	{
		struct FVector2I
		{
			int32 X;
			int32 Y;

			FVector2I(int32 InX, int32 InY)
				: X(InX), Y(InY)
			{
			};
		};

		TArray<FVector2I> GridList;
		GridList.Reserve(Width * 2 + Height * 2);

		// 가장자리 부분의 좌표 구하기
		for (int i = 0; i < Width; i++)
		{
			GridList.Add(FVector2I(i, 0));
			GridList.Add(FVector2I(i, Height - 1));
		}
		for (int i = 0; i < Height; i++)
		{
			GridList.Add(FVector2I(0, i));
			GridList.Add(FVector2I(Width - 1, i));
		}

		int32 Index = FMath::RandRange(0, GridList.Num() - 1);
		FVector2I GoalGridLocation = GridList[Index];
		ExitX = GoalGridLocation.X;
		ExitY = GoalGridLocation.Y;

		FVector Location = StartLocation + FVector(-GoalGridLocation.Y * CellSize, GoalGridLocation.X * CellSize, 0.0f);
		ExitActor = World->SpawnActor<AMazeExitActor>(	// 출구 엑터 스폰
			MazeExitActorClass,
			Location,
			FRotator::ZeroRotator);
	}

	// 미로 데이터를 이용하여 셀 생성
	if (CellActorClass)
	{			
		int32 StartX = static_cast<int32>(StartLocation.X) / (CellHalfSize * 2);
		int32 StartY = static_cast<int32>(-StartLocation.Y) / (CellHalfSize * 2);
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Start Location : %d, %d"), StartX, StartY));

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

						if (x == ExitX && y == ExitY && ExitActor)
						{
							CellActor->OnCellClear.AddDynamic(ExitActor, &AMazeExitActor::ActivateExit);	// 출구 셀이 클리어되면 출구 엑터의 활성화 함수 호출
						}
					}

					if (x == StartX && y == StartY)
					{
						CellActor->SetClear();			// 시작 셀은 Clear 상태로 시작
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
