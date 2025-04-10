#pragma once

#include "CoreMinimal.h"

// 디버프 타입 enum
UENUM(BlueprintType)
enum class EDebuffType : uint8
{
	None		UMETA(DisplayName = "None"),
	Slow		UMETA(DisplayName = "Slow"),		// 이동 속도 감소. 일정시간 동안 작동, 누적 안됨, 추가시 시간 연장
	Stun		UMETA(DisplayName = "Stun"),		// 스턴. 일정시간 동안 작동, 누적 안됨, 추가시 시간 연장
	DotDamage	UMETA(DisplayName = "DotDamage"),	// 지속 피해. 일정시간 동안 작동, 누적 됨, 추가시 시간 연장
	COUNT		UMETA(Hidden),						// 디버프 타입 개수
	// Add more debuff types as needed
};

UENUM(BlueprintType)
enum class EDebuffModifier : uint8
{
	Speed		UMETA(DisplayName = "Speed"),
	Damage		UMETA(DisplayName = "Damage"),
};