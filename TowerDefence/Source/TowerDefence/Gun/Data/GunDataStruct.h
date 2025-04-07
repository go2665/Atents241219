#pragma once

#include "CoreMinimal.h"
#include "GunDataStruct.generated.h"

USTRUCT(BlueprintType)
struct FGunDataStruct
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Data")
	FText GunName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Data")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Data")
	float FireRate;	// 공격 당 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun Data")
	float Range;	// cm 단위

	// 디버프용 enum, 단일/범위 공격 설정 enum, 속성 enum	
	
};