// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PoolingEnemyBase.h"
#include "PoolingEnemy_Shooter.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API APoolingEnemy_Shooter : public APoolingEnemyBase
{
	GENERATED_BODY()

public:
	APoolingEnemy_Shooter();

protected:
	virtual void OnActivate() override;

private:
	// 지정된 시간동안 앞으로 이동
	void EnterMoveState();

	// 회전 속도에 따라 목표 지점을 바라보게 하기
	void EnterLookAtState();

	// 총알을 지정된 회수만큼 연속 발사
	void EnterFireState();

	// 목표 지점을 바라보기
	void TurnToLookAtLocation();

	// 총알 한발 발사
	void FireOneShot();

	// 목표 지점을 바라보는 회전이 완료되었는지 확인
	bool IsTurnComplete() const;


protected:
	// 이동하는 시간
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float MoveDuration = 1.0f;

	// 목표로 회전하는 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float TurnSpeed = 360.0f;

	// 총알 연속 발사 개수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	int32 FireRepeatCount = 3;

	// 총알 연속 발사시 발사 간격
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pooling Enemy")
	float FireInterval = 0.2f;

private:
	// 타겟을 바라보는 처리를 위한 틱 발생용 타이머
	UPROPERTY()
	FTimerHandle LookAtTickTimer;

	// 발사 간격을 위한 타이머
	UPROPERTY()
	FTimerHandle FireIntervalTimer;

	// 바라볼 목표 위치
	FVector LookAtLocation;

	// 현재 발사한 회수
	int8 CurrentFireCount = 0;

	// 회전 틱 간격
	const float TurnTickInterval = 0.1f;
};
