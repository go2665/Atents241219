// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerGoldChange, int32, NewGold);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTION_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	bool AddItemToInventory(class UItemDataAsset* InItemDataAsset);

	inline int32 GetGold() const { return Gold; }
	inline void AddGold(int32 Amount) { SetGold(Gold + Amount); }

	FOnPlayerGoldChange OnGoldChange;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	inline void SetGold(int32 NewGold) { Gold = NewGold; OnGoldChange.Broadcast(Gold); }

	UFUNCTION()
	inline void TestGoldChange(int32 NewGold)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Gold changed to %d"), NewGold));
	}

public:	

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gold")
	int32 Gold = 0;

private:
	Inventory Inven;
		
};
