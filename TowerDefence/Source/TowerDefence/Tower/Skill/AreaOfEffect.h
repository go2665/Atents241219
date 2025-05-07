// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AreaOfEffect.generated.h"

class UCapsuleComponent;
class UNiagaraComponent;
class UTowerDamageType;
class USkillDataAsset;

UCLASS()
class TOWERDEFENCE_API AAreaOfEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAreaOfEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void OnInitialize(const USkillDataAsset& InSkillData, int32 InTowerLevel);

private:
	// 타겟과 겹쳤을 때 호출되는 함수
	UFUNCTION()
	void OnTargetOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 타겟이 나갔을 때 호출되는 함수
	UFUNCTION()
	void OnTargetOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnTickDamage(); // 틱 데미지 함수

protected:
	// 캡슐 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* Sensor; 

	// 나이아가라 이펙트 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* VFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AreaOfEffect")
	float Radius = 100.0f; // 반지름

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AreaOfEffect")
	float Damage = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AreaOfEffect")
	float DotDamage = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AreaOfEffect")
	float DotInterval = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AreaOfEffect")
	TSubclassOf<UTowerDamageType> DamageType;	// 데미지 타입

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bShowDebugInfo = false;				// 디버그 정보 표시 여부

	// 영향을 받은 액터들
	TArray<AActor*> AffectedActors; 

private:
	FTimerHandle TimerHandle;
};
