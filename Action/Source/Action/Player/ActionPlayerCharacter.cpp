// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AActionPlayerCharacter::AActionPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;	// 컨트롤러의 회전값을 사용하지 않음(3인칭 카메라에서는 사용하지 않음)
	GetCharacterMovement()->bOrientRotationToMovement = true;	// 이동 방향으로 캐릭터를 회전시킴
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);	// 회전 속도 설정(Yaw축만 사용)

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);				// 부모가 루트
	SpringArm->TargetArmLength = 350.0f;					// 암의 길이 설정
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 250.0f);	// 암의 위치 옵셋 설정
	SpringArm->ProbeSize = 25.0f;							// 암의 충돌 크기 설정(벽에 암이 부딛칠 때 등등)
	SpringArm->bUsePawnControlRotation = true;				// 암의 회전을 컨트롤러에 맞춤

	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(SpringArm);				// 부모가 스프링암
	PlayerCamera->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));	// 카메라의 기본 회전 설정
}

void AActionPlayerCharacter::Movement(const FVector& Direction)
{
	AddMovementInput(Direction);
}

void AActionPlayerCharacter::DoRoll()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Roll"));
	if (!GetMesh()->GetAnimInstance()->Montage_IsPlaying(RollMontage))
	{
		PlayAnimMontage(RollMontage);
	}
}

// Called when the game starts or when spawned
void AActionPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AActionPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AActionPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

