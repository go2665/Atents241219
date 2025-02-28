#pragma once
#include "CoreMinimal.h"
#include "WeaponType.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Axe = 0	UMETA(DisplayName = "Axe"),
	Club	UMETA(DisplayName = "Club"),
	Sword	UMETA(DisplayName = "Sword"),
	Max		UMETA(DisplayName = "Max")
};
