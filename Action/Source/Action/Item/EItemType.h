#pragma once
#include "CoreMinimal.h"
#include "EItemType.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	HealOrb	= 0,
	DealOrb,
	GoldCoin,
	Axe,
	Club,
	Sword,
	Gem,
	HealPotionSmall,
	HealPotionLarge,
	Apple,
	Banana,
};