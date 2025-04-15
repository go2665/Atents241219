// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TowerBuffComponent.generated.h"

// 주변 타워에 버프를 걸고, 자신에게 걸린 버프를 관리하는 컴포넌트

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERDEFENCE_API UTowerBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTowerBuffComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buff")
	class UTowerBuffDataAsset* BuffDataAsset = nullptr;	// 버프 데이터 에셋(null이 아니면 주기적으로 버프를 건다)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float Interval = 1.0f; // 버프 거는 주기(초 단위)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float Radius = 150.0f; // 버프 범위(타워 주변 반경)
};
