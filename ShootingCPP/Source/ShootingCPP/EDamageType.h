// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EDamageType.generated.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	DT_Red = 0	UMETA(DisplayName = "Red"),
	DT_Green	UMETA(DisplayName = "Green"),
	DT_Blue		UMETA(DisplayName = "Blue"),
	DT_Yellow	UMETA(DisplayName = "Yellow"),
};
