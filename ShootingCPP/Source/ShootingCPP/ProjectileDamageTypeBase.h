// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "EDamageType.h"
#include "ProjectileDamageTypeBase.generated.h"

/**
 * 
 */

UCLASS(Abstract)
class SHOOTINGCPP_API UProjectileDamageTypeBase : public UDamageType
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage Type")
	EDamageType Type = EDamageType::DT_Red;
};
