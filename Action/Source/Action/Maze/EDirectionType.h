#pragma once
#include "CoreMinimal.h"
#include "EDirectionType.generated.h"

UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EDirectionType : uint8
{
	None	= 0			UMETA(Hidden),
	North	= 1 << 0	UMETA(DisplayName = "North"),
	East	= 1 << 1	UMETA(DisplayName = "East"),
	South	= 1 << 2	UMETA(DisplayName = "South"),
	West	= 1 << 3	UMETA(DisplayName = "West")
};

// 비트플래그 연산자 오버로딩
ENUM_CLASS_FLAGS(EDirectionType)