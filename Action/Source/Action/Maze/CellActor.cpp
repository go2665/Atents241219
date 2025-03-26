// Fill out your copyright notice in the Description page of Project Settings.


#include "CellActor.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "CellData.h"
#include "../Enemy/EnemyBase.h"

// Sets default values
ACellActor::ACellActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	BackgroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BackgroundMesh"));	// 배경 매시 생성
	BackgroundMesh->SetupAttachment(RootComponent);
	BackgroundMesh->SetCollisionProfileName(TEXT("BlockAll"));	// 충돌 프로필 설정

	UStaticMeshComponent* Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateNorth"));	// 문 생성
	Gate->SetupAttachment(BackgroundMesh);				// 배경 매시에 붙임
	Gate->SetCollisionProfileName(TEXT("GateProfile"));	// 문의 충돌 프로필 설정
	Gate->SetRelativeLocation(FVector::ForwardVector * (CellHalfSize - GateHalfThickness));	// 문의 위치 설정
	GateMeshArray.Add(Gate);	// 문 배열에 추가해서 저장

	Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateEast"));
	Gate->SetupAttachment(BackgroundMesh);
	Gate->SetCollisionProfileName(TEXT("GateProfile"));
	Gate->SetRelativeLocation(FVector::RightVector * (CellHalfSize - GateHalfThickness));
	Gate->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
	GateMeshArray.Add(Gate);

	Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateSouth"));
	Gate->SetupAttachment(BackgroundMesh);
	Gate->SetCollisionProfileName(TEXT("GateProfile"));
	Gate->SetRelativeLocation(FVector::BackwardVector * (CellHalfSize - GateHalfThickness));
	Gate->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	GateMeshArray.Add(Gate);

	Gate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GateWest"));
	Gate->SetupAttachment(BackgroundMesh);
	Gate->SetCollisionProfileName(TEXT("GateProfile"));
	Gate->SetRelativeLocation(FVector::LeftVector * (CellHalfSize - GateHalfThickness));
	Gate->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
	GateMeshArray.Add(Gate);

	SensorCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SensorCollision"));	// 플레이어 감지용 센서 콜리젼
	SensorCollision->SetupAttachment(RootComponent);
	SensorCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SensorCollision->SetBoxExtent(FVector(CellHalfSize * 0.75f, CellHalfSize * 0.75f, 100.0f));	// 크기 설정
	SensorCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));							// 위치 설정

	UArrowComponent* Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(RootComponent);
	Arrow->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));

	this->SetCanBeDamaged(false);	// 데미지를 받지 않음
}

void ACellActor::Initialize(CellData* InCellData)
{
	if (InCellData)	// 셀 데이터를 받았을 때만 처리
	{
		Path = InCellData->GetPath();	// 길 정보 받아와서 저장
		OpenGate();						// 길 정보에 따라 문 열기	
	}
}

void ACellActor::BeginPlay()
{
	Super::BeginPlay();
	SensorCollision->OnComponentBeginOverlap.AddDynamic(this, &ACellActor::OnSensorBeginOverlap);
}

#if WITH_EDITOR
void ACellActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? 
		PropertyChangedEvent.Property->GetFName() : NAME_None;	// 변경된 프로퍼티의 이름 가져오기

	// EnemyCountMin나 EnemyCountMax가 변경되었을 때
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ACellActor, EnemyCountMin) ||
		PropertyName == GET_MEMBER_NAME_CHECKED(ACellActor, EnemyCountMax))
	{
		// EnemyCountMax가 무조건 Min보다 크거나 같도록 설정
		EnemyCountMax = FMath::Max(EnemyCountMin, EnemyCountMax);
	}
}
#endif

void ACellActor::OnSensorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 센서 안에 아직 "클리어가 되지 않았고" "문이 열려있고" "플레이어가 들어왔을 때"
	if (!bIsClear && bIsOpened && OtherActor->ActorHasTag("Player"))
	{
		CloseGate();	// 모든 문을 닫고
		SpawnEnemy();	// 적을 스폰
	}
}

void ACellActor::OnEnemyDie()
{
	SpawnCount--;	// 적이 죽었을 때 스폰 카운트 감소
	if (SpawnCount <= 0)	// 적이 모두 죽었으면
	{
		bIsClear = true;				// 클리어 상태로 변경
		OnCellClear.Broadcast(this);	// 클리어 이벤트 호출
		OpenGate();						// 모든 문을 열어줌
	}
}

void ACellActor::OpenGate()
{
	if (!bIsOpened)	// 문이 닫혀있을 때만 열기
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("OpenGate"));
		bIsOpened = true;
		for (int i = 0; i < 4; i++)	// 4방향에 대해서
		{
			if ((Path & static_cast<EDirectionType>(1 << i)) != EDirectionType::None)	// 비트 플래그가 켜져있으면
			{
				GateMeshArray[i]->SetVisibility(false);									// 문을 안보이게 하고
				GateMeshArray[i]->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 충돌을 없앰( = 문이 열린 것 처럼 보임)
			}
		}
	}
}

void ACellActor::CloseGate()
{
	if (bIsOpened)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("CloseGate"));
		bIsOpened = false;
		for (int i = 0; i < 4; i++)	// 모든 문을 닫기
		{
			GateMeshArray[i]->SetVisibility(true);
			GateMeshArray[i]->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
}

void ACellActor::SpawnEnemy()
{
	if (EnemyClass)
	{
		UWorld* World = GetWorld();
		SpawnCount = FMath::RandRange(EnemyCountMin, EnemyCountMax);	// Min ~ Max 사이의 랜덤한 수로 스폰 카운트 설정

		for (int i = 0; i < SpawnCount; i++)	// 스폰 카운트만큼 적 스폰
		{
			FVector Location = FMath::RandPointInBox(SensorCollision->Bounds.GetBox());
			Location.Z = 0.0f;	// 위치는 센서 콜리젼 안에서 랜덤하게(높이는 0)

			AEnemyBase* Enemy = World->SpawnActor<AEnemyBase>(EnemyClass, Location, FRotator::ZeroRotator);
			Enemy->OnDie.AddDynamic(this, &ACellActor::OnEnemyDie);	// 적이 죽었을 때 호출할 함수 설정
		}
	}
}
