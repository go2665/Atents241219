// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TowerDefence/Tower/Data/SkillDataAsset.h"
#include "TowerDefence/Components/Effect/EffectTargetable.h"
#include "SkillComponent.generated.h"

class APlayerSpectatorPawn;	// 플레이어 폰

/*
* 스킬 컴포넌트(HeroTower에 추가되어 스킬을 사용하는 컴포넌트, 하나의 스킬만 사용 가능)
*/

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERDEFENCE_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USkillComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 스킬을 사용할 수 있도록 플레이어에게 범위 결정용 액터를 켜게 만드는 함수(스킬 사용할 위치 선택 가능하게 만들기)
	UFUNCTION(BlueprintCallable, Category = "Hero|Skill")
	void ActivateSkill();

	// 지정된 위치에 스킬을 사용하는 함수
	UFUNCTION(BlueprintCallable, Category = "Hero|Skill")
	void UseSkill(FVector InLocation);

	// 스킬의 현재 쿨타임 비율을 리턴하는 함수
	inline float GetCoolTimeRatio() const { 
		return (SkillLevelData->CooldownTime - SkillCoolTime) / SkillLevelData->CooldownTime; 
	}

protected:

private:
	UFUNCTION()
	void OnSkillAreaClicked();	// 스킬 사용 위치 클릭 시 호출되는 함수

	void FindActorsInRadius(FVector Center, float Radius,
		TArray<TScriptInterface<IEffectTargetable>>& OutEffectTargets);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	USkillDataAsset* SkillData = nullptr;	// 스킬 데이터 에셋

private:
	// 플레이어 폰
	UPROPERTY()
	APlayerSpectatorPawn* PlayerPawn = nullptr;	

	// 스킬 레벨 데이터
	FSkillLevelData* SkillLevelData = nullptr;	

	// 스킬 사용 위치 선택을 위해 활성화 중인지를 표시하는 변수
	bool bIsSkillActivated = false;	

	// 스킬 쿨타임
	float SkillCoolTime = 0.0f;

	// 스킬을 사용한 타워의 레벨
	int32 TowerLevel = 0;

};
