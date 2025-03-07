// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemStructure.generated.h"

USTRUCT(BlueprintType)
struct FActionItem
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UStaticMesh* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (ClampMin = "0"))
	int32 ItemPrice = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (ClampMin = "1"))
	int32 ItemStackCount = 1;
	
	FActionItem()
	{
		ItemName = FText::FromString("아이템");
		ItemDescription = FText::FromString("아이템 설명");
		ItemMesh = nullptr;
		ItemPrice = 0;
		ItemStackCount = 1;
	}
};