// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolingEnemyBase.h"
#include "PoolingEnemy_Chase.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API APoolingEnemy_Chase : public APoolingEnemyBase
{
	GENERATED_BODY()

public:
	APoolingEnemy_Chase();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void OnActivate() override;
	virtual void OnDeactivate() override;

private:
	inline void TargetLost() { TargetPawn = nullptr; };

	// 플레이어를 계속 바라보는 함수
	void BehaviorPhase1(float DeltaTime);	

	// 앞쪽으로 가속시키는 함수
	void BehaviorPhase2(float DeltaTime);	

protected:
	UPROPERTY()
	APawn* TargetPawn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float RotateSpeed = 180.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float Phase1Duration = 5.0f;
};
