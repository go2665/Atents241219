// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TowerBuffEnums.h"
#include "TowerBuffBase.h"
#include "TowerBuffComponent.generated.h"

class ATowerBuilderActor; // 전방 선언(타워 배치 가능한 액터)
class UTowerBuffDataAsset;

// 주변 타워에 버프를 걸고, 자신에게 걸린 버프를 관리하는 컴포넌트

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERDEFENCE_API UTowerBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTowerBuffComponent();

	UFUNCTION(BlueprintCallable, Category = "Buff")
	void OnAddedBuff(const UTowerBuffDataAsset* Data);	// 버프 추가 되었을 때 실행할 일

	UFUNCTION(BlueprintCallable, Category = "Buff")
	void OnRemoveBuff(ETowerBuffType Type);	// 버프가 제거 되었을 때 실행할 일

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UFUNCTION()
	void AddBuffToAround();	
		
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Buff")
	UTowerBuffDataAsset* BuffDataAsset = nullptr;	// 버프 데이터 에셋(null이 아니면 주기적으로 버프를 건다)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float Interval = 1.0f; // 버프 거는 주기(초 단위)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff")
	float Radius = 150.0f; // 버프 범위(타워 주변 반경)

private:
	UPROPERTY(VisibleAnywhere, Category = "Buff")
	TArray<UTowerBuffBase*> BuffList;

	UTowerBuffBase* CreateBuff(ETowerBuffType Type); // 버프 생성

	TArray<int32> RemoveIndices;  // 매 틱마다 제거할 디버프 인덱스 저장(재할당을 방지하기 위해 맴버변수로 선언)

	// 주변에 있는 타워 배치 가능한 액터(ATowerBuilderActor)를 저장하는 배열
	UPROPERTY()
	TArray<ATowerBuilderActor*> TowerBuilderList;	// 타워 배치 가능한 액터 리스트(시작하면 변하지 않는다)

	FTimerHandle BuffTimerHandle; // 버프 타이머 핸들
};
