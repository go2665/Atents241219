// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DummyEnemy.h"
#include "DummyEnemy_DropItem.generated.h"


/**
 * 
 */
UCLASS()
class ACTION_API ADummyEnemy_DropItem : public ADummyEnemy
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "DropItem")
	void TestDropItem();

protected:
	
};
