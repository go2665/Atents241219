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
	// 플레이어 사망시 원할한 처리를 위한 함수
	UFUNCTION()
	inline void TargetLost() { TargetPawn = nullptr; };

	// 플레이어를 계속 바라보는 함수
	void BehaviorPhase1(float DeltaTime);	

	// 앞쪽으로 가속시키는 함수
	void BehaviorPhase2(float DeltaTime);	

protected:
	// 추적할 플레이어
	UPROPERTY()
	APawn* TargetPawn = nullptr;

	// 페이즈1에서의 회전 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float RotateSpeed = 180.0f;

	// 페이즈1 지속 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float Phase1Duration = 5.0f;

	// 최대 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float MaxSpeed = 1500.0f;

	// 페이즈2에서의 가속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float Acceleration = 500.0f;

	// 페이즈1에서의 데미지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float Phase1Damage = 10.0f;

	// 페이즈2에서의 데미지
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float Phase2Damage = 500.0f;

private:
	// 페이즈1 진입했을 때 한번 실행하기 위한 불리언 변수	
	bool bIsPhase1 = false;

	// 페이즈2 진입했을 때 한번 실행하기 위한 불리언 변수
	bool bIsPhase2 = false;
};
