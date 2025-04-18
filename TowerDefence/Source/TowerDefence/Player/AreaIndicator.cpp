// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaIndicator.h"
#include "TowerDefence/Framework/TowerDefencePlayerController.h"

// Sets default values
AAreaIndicator::AAreaIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 루트 만들기
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	// 스태틱 메시 만들기
	AreaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AreaMesh"));
	AreaMesh->SetupAttachment(Root); // 루트에 스태틱 메시 붙이기
	AreaMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화
}

// Called when the game starts or when spawned
void AAreaIndicator::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = GetWorld()->GetFirstPlayerController(); // 플레이어 컨트롤러 가져오기
	SetAreaRadius(AreaRadius); // 초기 AreaRadius 설정

	ATowerDefencePlayerController* TowerDefenceController = Cast<ATowerDefencePlayerController>(PlayerController);
	if (TowerDefenceController)
	{
		TowerDefenceController->OnMouseClickInput.AddDynamic(this, &AAreaIndicator::OnMouseClick); // 마우스 클릭 이벤트 바인딩
	}

	Deactivate(); // 시작 시 AreaIndicator 비활성화
}

// Called every frame
void AAreaIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateLocation();
}

void AAreaIndicator::SetAreaRadius(float NewRadius)
{
	AreaRadius = NewRadius;

	// AreaRadius에 따라 스태틱 메시 크기 조정
	FVector NewScale = FVector(AreaRadius * ScaleFactor);
	AreaMesh->SetWorldScale3D(NewScale);
}

void AAreaIndicator::Activate()
{
	UpdateLocation();				// 현재 위치로 업데이트
	SetActorHiddenInGame(false);	// AreaIndicator 보이게 하기
	SetActorTickEnabled(true);		// Tick 활성화
}

void AAreaIndicator::Deactivate()
{
	SetActorHiddenInGame(true); // AreaIndicator 안보이게 하기
	SetActorTickEnabled(false); // Tick 비활성화
}

void AAreaIndicator::OnMouseClick(AActor* HitActor)
{
	Deactivate();
	//UE_LOG(LogTemp, Warning, TEXT("AAreaIndicator::OnMouseClick"));
}

void AAreaIndicator::UpdateLocation()
{
	// 마우스 위치에 따라 AreaIndicator 위치를 업데이트(높이는 +300)
	FHitResult HitResult;
	if (PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, HitResult))
	{
		FVector NewLocation = HitResult.ImpactPoint;
		NewLocation.Z = HeightOffset;	// Z축 높이 조정
		SetActorLocation(NewLocation);	// AreaIndicator 위치 설정
	}
}