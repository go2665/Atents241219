// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ActionPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerGoldChange, int32, NewGold);

/**
 * 
 */
UCLASS()
class ACTION_API AActionPlayerState : public APlayerState
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	inline int32 GetGold() const { return Gold; }
	inline void AddGold(int32 Amount) { SetGold(Gold + Amount); }

	FOnPlayerGoldChange OnGoldChange;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 Gold = 0;

private:
	inline void SetGold(int32 NewGold) { Gold = NewGold; OnGoldChange.Broadcast(Gold); }

	UFUNCTION()
	inline void TestGoldChange(int32 NewGold)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Gold changed to %d"), NewGold));
	}	
};
