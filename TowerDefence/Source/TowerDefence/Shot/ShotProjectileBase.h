// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TowerDefence/Shot/Data/ProjectileShotDataAsset.h"
#include "ShotProjectileBase.generated.h"

UCLASS()
class TOWERDEFENCE_API AShotProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AShotProjectileBase();
	virtual void OnConstruction(const FTransform& Transform) override;	// 액터가 생성될 때 호출되는 함수
	virtual void Tick(float DeltaTime) override;	// 매 프레임 호출되는 함수

protected:
	virtual void BeginPlay() override;	// 액터가 시작될 때 호출되는 함수

public:	
	// 발사체 데이터 초기화(스폰 직후에 반드시 호출 되어야 함)
	UFUNCTION(BlueprintCallable, Category = "Shot")
	void InitializeShotData(AActor* Target, UProjectileShotDataAsset* NewShotData);	

	UFUNCTION()
	inline void AddHitEnemy(AActor* HitEnemy) { HitEnemies.Add(HitEnemy); };

protected:
	UFUNCTION()
	void OnOverlapEnemy(AActor* OverlappedActor, AActor* OtherActor);	// 적과 충돌 시 호출되는 함수

	virtual void OnHitEnemy(class AEnemyBase* HitEnemy);	// 적과 충돌 시 호출되는 함수

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* ProjectileMovement = nullptr;	// 발사체 이동 컴포넌트

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Mesh = nullptr;	// 발사체 메쉬

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shot")
	UProjectileShotDataAsset* ShotData = nullptr;	// 발사체 데이터

private:
	AActor* TargetActor = nullptr;					// 발사체가 날아갈 타겟 액터(적)
	FVector TargetLocation = FVector::ZeroVector;	// 발사체가 날아갈 목표 위치
	TArray<AActor*> HitEnemies;						// 이 발사체를 맞은 적들
};
