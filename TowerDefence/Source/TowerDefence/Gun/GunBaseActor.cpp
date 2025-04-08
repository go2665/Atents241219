// Fill out your copyright notice in the Description page of Project Settings.


#include "GunBaseActor.h"

// Sets default values
AGunBaseActor::AGunBaseActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(Root);
	GunMesh->SetRelativeScale3D(FVector(ScaleFactor));
	GunMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);	

	GunDatas.Reserve(3); // 레벨 1~3까지의 총기 데이터 배열 크기 설정
}

// Called when the game starts or when spawned
void AGunBaseActor::BeginPlay()
{
	Super::BeginPlay();

	//check(GunDatas[0] != nullptr);		// 조건이 충족하지 않으면 종료
	ensure(GunDatas[0] != nullptr);			// 조건이 충족하지 않으면 경고 메시지 출력 후 계속 진행
	//verify(GunDatas[0] != nullptr);		// 조건이 충족하지 않으면 종료(릴리즈에서는 생략)
	CurrentGunData = GunDatas[0]; // 기본 총기 데이터 선택
}

// Called every frame
void AGunBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGunBaseActor::SetGunLevel(int Level)
{
	if (Level > 0 && Level <= GunDatas.Num())
	{
		CurrentGunData = GunDatas[Level - 1];	// 레벨에 맞는 총기 데이터 선택
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid gun level: %d"), Level);
	}
}

