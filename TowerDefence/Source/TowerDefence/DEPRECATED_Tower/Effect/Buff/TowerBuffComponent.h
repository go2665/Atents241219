// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TowerDefence/DEPRECATED_Tower/Defines/TowerBuffEnums.h"
#include "TowerBuffBase.h"
#include "TowerBuffComponent.generated.h"

class ATowerBuilderActor; // 전방 선언(타워 배치 가능한 액터)
class UTowerBuffDataAsset;

// 주변 타워에 버프를 걸고, 자신에게 걸린 버프를 관리하는 컴포넌트

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERDEFENCE_API UTowerBuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTowerBuffComponent();

	// 버프 추가 되었을 때 실행할 일
	void OnAddedBuff(const UTowerBuffDataAsset * InData);	

	// 버프가 제거 되었을 때 실행할 일
	UFUNCTION(BlueprintCallable, Category = "Tower|Buff")
	void OnRemoveBuff(ETowerBuffType Type);	

	// 모디파이어가 제공하는 배율 리턴
	inline float GetBuffModifierValue(ETowerBuffModifier Type) const
	{
		if (TotalBuffModifiers.Contains(Type))
		{
			return TotalBuffModifiers[Type];
		}
		return 1.0f; // 기본값
	}

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// 주변 타워에 버프를 건다(타워 배치 가능한 액터를 순회하면서 버프를 건다)
	UFUNCTION()
	void AddBuffToAround();	

	// 버프 생성
	UTowerBuffBase* CreateBuff(ETowerBuffType Type);

	// 현재 적용된 버프의 모디파이어를 합산하는 함수
	void CalculateTotalBuffModifiers(); 
		
protected:
	// 버프 데이터 에셋(null이 아니면 주기적으로 버프를 건다, 버프 주는 타워인지 결정)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tower|Buff")
	UTowerBuffDataAsset* BuffDataAsset = nullptr;	

	// 버프 거는 주기(초 단위)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Buff")
	float Interval = 1.0f; 

	// 버프 범위(타워 주변 반경)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tower|Buff")
	float Radius = 150.0f; 

private:
	// 현재 적용된 버프 리스트(Owner타워에 적용된 버프 리스트)
	UPROPERTY(VisibleAnywhere, Category = "Tower|Buff")
	TArray<UTowerBuffBase*> BuffList;	

	// 주변에 있는 타워 배치 가능한 액터(ATowerBuilderActor)를 저장하는 배열(시작하면 변하지 않는다)
	UPROPERTY(VisibleAnywhere, Category = "Tower|Buff")
	TArray<ATowerBuilderActor*> TowerBuilderList;

	// 타워에 적용된 버프의 모디파이어를 합산한 값
	UPROPERTY(VisibleAnywhere, Category = "Tower|Buff")
	TMap<ETowerBuffModifier, float> TotalBuffModifiers;

	// 매 틱마다 제거할 디버프 인덱스 저장(재할당을 방지하기 위해 맴버변수로 선언)
	TArray<int32> RemoveIndices;  

	// 버프 타이머 핸들
	FTimerHandle BuffTimerHandle; 

	class ATowerBaseActor* OwnerTower = nullptr;	// 이 컴포넌트가 붙어 있는 타워(Owner타워)

};
