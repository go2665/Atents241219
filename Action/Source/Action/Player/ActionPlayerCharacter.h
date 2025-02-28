// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Weapon/WeaponType.h"
#include "ActionPlayerCharacter.generated.h"

UCLASS()
class ACTION_API AActionPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AActionPlayerCharacter();

	// 컨트롤러에서 입력 방향을 받아서 처리하는 함수
	UFUNCTION(BlueprintCallable, Category = "Player Movement")
	void Movement(const FVector& Direction);

	UFUNCTION(BlueprintCallable, Category = "Player Movement")
	void DoRoll();

	UFUNCTION(BlueprintCallable, Category = "Player Attack")
	void DoAttack();

	UFUNCTION()
	void OnSectionJumpReady(class UANS_SectionJump* SectionJump);

	UFUNCTION()
	void OnSectionJumpEnd();

	UFUNCTION()
	void SetCurrentWeaponCollisionActivate(bool bActivate);

	UFUNCTION(BlueprintCallable, Category = "Player Weapon")
	void SetCurrentWeapon(EWeaponType WeaponType);

	// 달리기 모드로 설정
	UFUNCTION(BlueprintCallable, Category = "Player Movement")
	inline void SetSprintMode() { GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; };

	// 걷기 모드로 설정
	UFUNCTION(BlueprintCallable, Category = "Player Movement")
	inline void SetWalkMode() { GetCharacterMovement()->MaxWalkSpeed = WalkSpeed; };	

protected:
	void PlayHighPriorityMontage(UAnimMontage* Montage, FName StartSectionName = NAME_None);

private:
	void SectionJumpForCombo();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Camera")
	class USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Camera")
	class UCameraComponent* PlayerCamera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Weapon")
	class UWeaponManagerComponent* WeaponManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement")
	float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement")
	float SprintSpeed = 1250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Movement")
	UAnimMontage* RollMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player Attack")
	UAnimMontage* AttackMontage = nullptr;

	UPROPERTY()
	class AWeaponActor* CurrentWeapon = nullptr;

private:
	// 콤보용 노티파이
	UPROPERTY()
	class UANS_SectionJump* SectionJumpNotify = nullptr;
	
	UPROPERTY()
	UAnimInstance* AnimInstance = nullptr;

	UPROPERTY()
	UAnimMontage* CurrentMontage = nullptr;

	// 콤보가 가능한 상황인지 확인하기 위한 플래그(true면 가능, false면 불가능)
	bool bIsComboReady = false;

};
