// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "DummyEnemy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTakeDamage, float, Damage);

UCLASS()
class ACTION_API ADummyEnemy : public AEnemyBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADummyEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 데미지 입었을 때 호출
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	// 데미지를 입었을 때 데미지 팝업을 띄우기 위한 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "DummyEnemy")
	FOnTakeDamage OnTakeDamage;

protected:
	// 팝업 데미지용 위젯 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWidgetComponent* PopupDamageWidget = nullptr;

private:
	// 팝업 데미지 위젯 인스턴스(델리게이트 바인딩 용)
	UPROPERTY()
	class UUserWidget_PopupDamage* PopupDamageWidgetInstance = nullptr;

};
