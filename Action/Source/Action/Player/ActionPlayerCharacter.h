// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Weapon/WeaponType.h"
#include "../Weapon/WeaponActor.h"
#include "ActionPlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChange, float, Health);

UCLASS()
class ACTION_API AActionPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	FOnHealthChange OnHealthChange;

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

	// 플레이어의 체력을 회복시키는 함수(전체 회복량, 회복 시간(회복시간이 0보다 작으면 즉시 회복)
	void RestoreHealth(float Health, float Duration = -1.0f);

	// Getter

	// 범위 공격용 데미지
	inline float GetAreaDamage() const { return CurrentWeapon != nullptr ? CurrentWeapon->GetDamage() * 2.0f : 1.0f; };
	
	// 최대 체력
	inline float GetMaxHealth() const { return MaxHealth; };

	// 현재 체력
	inline float GetCurrentHealth() const { return CurrentHealth; };


	// Setter
	
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

	// CurrentHealth변경은 이 함수를 통해서만 하는 것이 원칙
	inline void SetCurrentHealth(float Health) { 
		CurrentHealth = Health;		// CurrentHealth 변경
		if (CurrentHealth < 0)		// 체력이 0보다 작으면
		{
			// 사망 처리
		}
		else
		{
			CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);	// 적정 범위로 클램프
			OnHealthChange.Broadcast(CurrentHealth); // CurrentHealth 변경 알림
		}
	};

	// OnHealthChange에 붙여서 테스트용으로 사용할 함수
	UFUNCTION()
	inline void TestPrintHealth(float Health) {
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red,
			FString::Printf(TEXT("Health : %.1f / %.1f"), CurrentHealth, MaxHealth));
	};

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	



protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Camera")
	class USpringArmComponent* SpringArm = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Camera")
	class UCameraComponent* PlayerCamera = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Weapon")
	class UWeaponManagerComponent* WeaponManager = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement")
	float WalkSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement")
	float SprintSpeed = 1250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Movement")
	UAnimMontage* RollMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Attack")
	UAnimMontage* AttackMontage = nullptr;

	UPROPERTY()
	class AWeaponActor* CurrentWeapon = nullptr;

	// 최대 체력
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Health")
	float MaxHealth = 100.0f;

	// 현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Health")
	float CurrentHealth = 100.0f;

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

	// 지속 회복 데이터
	struct FRestoreData
	{
		FRestoreData(float InRestoreHealthPerSec, float InDuration, float InElapsedTime)
			: RestoreHealthPerSec(InRestoreHealthPerSec), Duration(InDuration), ElapsedTime(InElapsedTime) {
		}
		float RestoreHealthPerSec = 0.0f;
		float Duration = 0.0f;
		float ElapsedTime = 0.0f;
	};

	// 틱에서 처리할 지속 회복 데이터	
	TArray<FRestoreData> RestoreDatas;

};
