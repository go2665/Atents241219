// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EEnemyType.generated.h"

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	ET_Straight = 0	UMETA(DisplayName = "Straight"),
	ET_Wave			UMETA(DisplayName = "Wave"),
	ET_Shooter		UMETA(DisplayName = "Shooter"),
	ET_Chase		UMETA(DisplayName = "Chase"),
	ET_Debris		UMETA(DisplayName = "Debris"),
};