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
	// Add more debuff types as needed
};

// 속성 타입 enum	
UENUM(BlueprintType)
enum class EAttributeType : uint8
{
	None		UMETA(DisplayName = "None"),
	Fire		UMETA(DisplayName = "Fire"),
	Ice			UMETA(DisplayName = "Ice"),
	Lightning	UMETA(DisplayName = "Lightning"),
	Poison		UMETA(DisplayName = "Poison"),
	// Add more attribute types as needed
};
	
