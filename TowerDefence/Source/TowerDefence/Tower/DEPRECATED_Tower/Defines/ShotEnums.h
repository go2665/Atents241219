#pragma once

#include "CoreMinimal.h"

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
	
