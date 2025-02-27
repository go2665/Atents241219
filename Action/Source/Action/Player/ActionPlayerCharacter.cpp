// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionPlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../AnimNotify/ANS_SectionJump.h"
#include "../Weapon/WeaponActor.h"

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
	AddMovementInput(Direction);	// 입력받은 방향으로 이동
}

void AActionPlayerCharacter::DoRoll()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Roll"));

	// 마지막 입력 방향으로 즉시 회전 시키기
	SetActorRotation(GetLastMovementInputVector().Rotation(), ETeleportType::ResetPhysics);
	
	// 몽타주 재생
	PlayHighPriorityMontage(RollMontage);
}

void AActionPlayerCharacter::DoAttack()
{
	// 몽타주 재생
	if (!AnimInstance->IsAnyMontagePlaying())	// Montage가 재생 중이 아니면
	{
		PlayHighPriorityMontage(AttackMontage);	// 공격용 몽타주 재생
	}
	else if (CurrentMontage == AttackMontage)	// 현재 재생 중인 몽타주가 공격용 몽타주면
	{
		SectionJumpForCombo();					// 콤보용 섹션 점프	
	}
}

void AActionPlayerCharacter::OnSectionJumpReady(UANS_SectionJump* SectionJump)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Cyan, TEXT("Section Jump Ready"));
	bIsComboReady = true;					// 콤보공격이 가능하다고 설정
	SectionJumpNotify = SectionJump;
}

void AActionPlayerCharacter::OnSectionJumpEnd()
{
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("Section Jump End"));
	SectionJumpNotify = nullptr;
	bIsComboReady = false;					// 콤보공격이 불가능하다고 설정
}

void AActionPlayerCharacter::SetCurrentWeaponActivation(bool bActivate)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->SetWeaponActivation(bActivate);
	}
}

// Called when the game starts or when spawned
void AActionPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	AnimInstance = GetMesh()->GetAnimInstance();	// 애님 인스턴스 캐싱
}

void AActionPlayerCharacter::PlayHighPriorityMontage(UAnimMontage* Montage, FName StartSectionName)
{
	if (!AnimInstance->Montage_IsPlaying(Montage))	// Montage가 재생 중이 아니면
	{
		CurrentMontage = Montage;					// Montage를 CurrentMontage로 지정
		PlayAnimMontage(CurrentMontage, 1.0f, StartSectionName);	// Montage 재생
	}
}

void AActionPlayerCharacter::SectionJumpForCombo()
{
	if (SectionJumpNotify && bIsComboReady)			// SectionJumpNotify가 있고 콤보가 가능하면(AnimNotifyState가 시작되었으면)
	{
		UAnimMontage* Current = AnimInstance->GetCurrentActiveMontage();
		AnimInstance->Montage_SetNextSection(
			AnimInstance->Montage_GetCurrentSection(Current),	// 현재 섹션
			SectionJumpNotify->GetNextSectionName(),			// 다음 섹션
			Current);											// 이 몽타주에서
		bIsComboReady = false;									// 중복실행 방지를 위해 콤보가 불가능하다고 설정
	}
}

// Called every frame
void AActionPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

