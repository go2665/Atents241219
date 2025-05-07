// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TowerDefence/Defines/TowerDefenceEnums.h"
#include "TowerDefence/Tower/Data/EffectDataAsset.h"
#include "TowerDefence/Tower/Effect/EffectBase.h"
#include "EffectComponent.generated.h"

class UEffectBase;			// 각종 버프/디버프 구현 기본 클래스
class IEffectTargetable;	// 이팩트가 적용될 타겟을 나타내는 인터페이스

USTRUCT(BlueprintType)
struct FEffectArray
{
	GENERATED_BODY()

	// 이팩트 리스트
	UPROPERTY(VisibleAnywhere)
	TArray<UEffectBase*> EffectList;	

	// 이팩트 비주얼 이펙트 컴포넌트
	UPROPERTY()
	UNiagaraComponent* EffectVFX = nullptr;
};

// 이팩트 변경 델리게이트(전체 이팩트 목록을 전달해야함)
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnEffectChanged, const TArray<UEffectBase*>&, InEffectList);	

/*
* 타워 이팩트 컴포넌트(이팩트를 관리하는 컴포넌트. 버프/디버프 추가 및 삭제, 틱 처리 등)
*/
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWERDEFENCE_API UEffectComponent : public UActorComponent
{
	GENERATED_BODY()

	// 컴포넌트는 이팩트만 관리(추가/삭제 등), 컴포넌트는 블루프린트에서 추가(클래스의 기본값으로 넣지 않아야함)
	// 이팩트가 추가/삭제 되었을 때 타워에 델리게이트로 알림

public:	
	// Sets default values for this component's properties
	UEffectComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 이팩트 추가 함수
	UFUNCTION(BlueprintCallable, Category = "Effect")
	bool AddEffect(EEffectType InType, int32 InLevel);

	// 이팩트 한 종류 제거하는 함수
	UFUNCTION(BlueprintCallable, Category = "Effect")
	bool RemoveEffect(EEffectType InType);

	UFUNCTION(BlueprintCallable, Category = "Effect")
	bool RemoveTargetEffect(UEffectBase* InRemoveEffect);

private:
	// 버프 인스턴스 생성
	UEffectBase* CreateEffect(EEffectType InType);

	// 현재 적용된 이팩트의 모디파이어를 합산하고 대상에게 적용하는 함수
	void ApplyTotalModifiersToTarget();

public:
	// 이팩트 변경 델리게이트(전체 이팩트 목록을 전달해야함)
	// FOnEffectChanged OnEffectChanged;

protected:
	// 게임에서 사용되는 모든 종류의 이팩트가 설정되어야 한다.(이팩트 타입과 이팩트 데이터 매핑)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	TMap<EEffectType, UEffectDataAsset*> EffectDataMap;	

	// 디버그 정보 표시 여부
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Debug")
	bool bShowDebugInfo = false;

private:
	//// 현재 적용된 이팩트 리스트(타워에 적용 될 버프 리스트)
	//UPROPERTY(VisibleAnywhere, Category = "Effect")
	//TArray<UEffectBase*> EffectList;
	
	// 이팩트 타입별로 이팩트 배열을 관리하기 위한 맵
	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TMap<EEffectType, FEffectArray> EffectTypeMap;

	// 모든 이팩트의 모디파이어가 합산된 값
	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TMap<EEffectModifier, float> TotalModifiers;

	// 이팩트의 효과가 적용될 대상(이 컴포넌트를 가지고 있는 타워)
	UPROPERTY(VisibleAnywhere, Category = "Effect")
	TScriptInterface<IEffectTargetable> EffectTarget = nullptr;

	// 이팩트의 시리얼 넘버(이팩트가 생성될 때마다 증가함)
	int32 SerialNumber = 0;	
};
