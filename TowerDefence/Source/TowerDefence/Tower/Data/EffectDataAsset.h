// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NiagaraSystem.h"
#include "EffectDataAsset.generated.h"

class UEffectBase;

// 레벨별 데이터를 가지는 구조체
USTRUCT(BlueprintType)
struct FEffectLevelData
{
	GENERATED_BODY()

public:
	// 이팩트 기본 지속 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect|Level Data")
	float Duration = 1.0f;

	// 이팩트 기본 스택 회수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect|Level Data", meta = (ClampMin = "1"))
	int32 MaxStackCount = 1;

	// 이팩트 수치 1(변화 비율. 예) 0.5면 50% 증가, -0.3이면 30% 감소)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect|Level Data")
	float Modifier1 = 0.0f;

	// 이팩트 수치 2
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect|Level Data")
	float Modifier2 = 0.0f;

	// 이팩트 수치 3
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect|Level Data")
	float Modifier3 = 0.0f;
};

/**
 * 
 */
UCLASS(Blueprintable)
class TOWERDEFENCE_API UEffectDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// 이팩트 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FText BuffName;	

	// 이팩트 특수효과
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* EffectVFX = nullptr;

	// 이팩트 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TSubclassOf<UEffectBase> EffectClass;	

	// 레벨별 데이터(지속시간, 스택 회수, 수치1, 수치2, 수치3)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TArray<FEffectLevelData> LevelData;		
};
