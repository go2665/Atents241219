#pragma once

#include "CoreMinimal.h"

// 버프 타입 enum
UENUM(BlueprintType)
enum class ETowerBuffType : uint8
{
	None		UMETA(DisplayName = "None"),
	RapidFire	UMETA(DisplayName = "RapidFire"),	// 발사 속도 증가. 일정시간 동안 작동, 누적 안됨, 추가시 시간 리셋
	LongRange	UMETA(DisplayName = "LongRange"),	// 사거리 증가. 일정시간 동안 작동, 누적 안됨, 추가시 시간 리셋
	PowerUp		UMETA(DisplayName = "PowerUp"),		// 공격력 증가. 일정시간 동안 작동, 누적 안됨, 추가시 시간 리셋
	Hero		UMETA(DisplayName = "Hero"),		// 영웅 버프. 일정시간 동안 작동, 누적 안됨, 추가시 시간 리셋
	COUNT		UMETA(Hidden),						// 디버프 타입 개수
	// Add more buff types as needed
};

UENUM(BlueprintType)
enum class ETowerBuffModifier : uint8
{
	FireRate	UMETA(DisplayName = "FireRate"),
	Range		UMETA(DisplayName = "Range"),
	Damage		UMETA(DisplayName = "Damage"),
	COUNT		UMETA(Hidden),	// 버프 타입 개수
};