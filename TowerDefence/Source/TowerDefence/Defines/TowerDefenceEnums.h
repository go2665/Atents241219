#pragma once

#include "CoreMinimal.h"

// 공격 우선 순위 타입 enum	
UENUM(BlueprintType)
enum class EFireOrder : uint8
{
	Discovery = 0	UMETA(DisplayName = "Discovery"),	// 발견 순서
	Distance		UMETA(DisplayName = "Distance"),	// 거리 순서
};

// 대미지 속성 타입 enum	
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	None		UMETA(DisplayName = "None"),
	Fire		UMETA(DisplayName = "Fire"),
	Ice			UMETA(DisplayName = "Ice"),
	Lightning	UMETA(DisplayName = "Lightning"),
	Poison		UMETA(DisplayName = "Poison"),
	// Add more attribute types as needed
};