// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCamera.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMainCamera::AMainCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);		// RootComponent : 이 엑터의 루트 컴포넌트
	Camera->SetRelativeLocationAndRotation(FVector(0, 0, 1000), FRotator(-90, 0, 0));	// 카메라 기본 회전과 이동 배치
	Camera->SetProjectionMode(ECameraProjectionMode::Orthographic);	// 직교 투영 모드
	Camera->SetOrthoWidth(1000);									// 직교 투영 모드의 너비
}

// Called when the game starts or when spawned
void AMainCamera::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	PlayerController->SetViewTargetWithBlend(this, 0.5f, EViewTargetBlendFunction::VTBlend_Cubic);	

	/*GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Yellow,
		FString::Printf(TEXT("Main Camera OrthoWidth : %.1f"), Camera->OrthoWidth));*/
}
