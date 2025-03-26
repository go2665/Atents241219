// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTakeDamage, float, Damage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDie);

UCLASS()
class ACTION_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

public:	
	//virtual void Tick(float DeltaTime) override;
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostDieAnimation();

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void Die();
	
	void DropItems();

public:
	// 죽었을 때 알리기 위한 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Enemy")
	FOnDie OnDie;

	// 데미지를 입었을 때 데미지 팝업을 띄우기 위한 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Enemy")
	FOnTakeDamage OnTakeDamage;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
	float MaxHealth = 10.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Enemy")
	float CurrentHealth = 10.0f;

	// 드랍 아이템 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop Item")
	class UDataTable* DropItemDataTable = nullptr;

	// 아이템을 얼마나 멀리 던질지 결정하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Drop Item")
	float ItemLaunchPower = 500.0f;

	// 피격 애니메이션 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* HitMontage = nullptr;

	// 사망 애니메이션 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	class UAnimMontage* DeathMontage = nullptr;

	// 팝업 데미지용 위젯 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWidgetComponent* PopupDamageWidget = nullptr;

	// 팝업 데미지 위젯 인스턴스(델리게이트 바인딩 용)
	UPROPERTY()
	class UUserWidget_PopupDamage* PopupDamageWidgetInstance = nullptr;

	// 생존 여부
	bool bIsAlive = true;

};
