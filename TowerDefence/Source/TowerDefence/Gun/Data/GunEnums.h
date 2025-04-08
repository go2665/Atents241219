#pragma once

#include "CoreMinimal.h"

// 디버프 타입 enum
UENUM(BlueprintType)
enum class EDebuffType : uint8
{
	None		UMETA(DisplayName = "None"),
	Slow		UMETA(DisplayName = "Slow"),
	Stun		UMETA(DisplayName = "Stun"),
	DotDamage	UMETA(DisplayName = "DotDamage"),
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
	
