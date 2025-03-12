// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DropItemBase.h"
#include "DropItem_Pickup.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API ADropItem_Pickup : public ADropItemBase
{
	GENERATED_BODY()

protected:
	virtual void OnGetItem(AActor* InTarget) override;
};
