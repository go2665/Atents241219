// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DebuffBase.h"
#include "DebuffEnums.h"
#include "DebuffComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERDEFENCE_API UDebuffComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDebuffComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void AddDebuff(EDebuffType Type);		// 디버프 추가

	UFUNCTION(BlueprintCallable, Category = "Debuff")
	void RemoveDebuff(EDebuffType Type);	// 디버프 제거

	float GetMaxiumModifierValue(EDebuffType IgnoreType, EDebuffModifier ModifierType);	// 최대 수정 값 반환

private:
	UPROPERTY(VisibleAnywhere, Category = "Debuff")
	TArray<UDebuffBase*> ActiveDebuffs;

	UDebuffBase* CreateDebuff(EDebuffType Type); // 디버프 생성

	TArray<int32> RemoveIndices;  // 매 틱마다 제거할 디버프 인덱스 저장(재할당을 방지하기 위해 맴버변수로 선언)
};
