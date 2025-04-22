#pragma once

#include "CoreMinimal.h"

// 속성 타입 enum	
UENUM(BlueprintType)
enum class EFireOrder : uint8
{
	Discovery = 0	UMETA(DisplayName = "Discovery"),	// 발견 순서
	Distance		UMETA(DisplayName = "Distance"),	// 거리 순서
};
