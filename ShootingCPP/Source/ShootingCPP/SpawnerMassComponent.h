// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnerBaseComponent.h"
#include "SpawnerMassComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGCPP_API USpawnerMassComponent : public USpawnerBaseComponent
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	virtual void Spawn() override;
	virtual FTransform GetSpawnTransform() const override;

public:
	// 한번에 생성할 적의 수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	int32 MassCount = 3;

	// 생성할 적들의 위치를 설정하기 위한 커브
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	UCurveFloat* FormationCurve = nullptr;

protected:
	// 생성할 적들이 등장할 전체 넓이
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawner")
	FVector2D Offset = FVector2D(100,300);

private:
	// 커브의 간격(매번 계산하지 않도록 BeginPlay에서 계산)
	float CurveInputGap = 0.0f;

	// 한번에 생성할 때 생성되는 순서를 나타내는 인덱스
	int32 SpawnIndex = 0;
};
