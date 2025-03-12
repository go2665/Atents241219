// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DropItem_Instance.h"
#include "DropItem_InstanceGold.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API ADropItem_InstanceGold : public ADropItem_Instance
{
	GENERATED_BODY()
	
protected:
	virtual void OnGetItem(AActor* InTarget) override;
};
