// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Inventory.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerGoldChange, int32, NewGold);


/// <summary>
/// Inventory 클래스를 사용하기 쉽게 래핑하고 있는 컴포넌트
/// </summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTION_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	// 아이템을 인벤토리에 추가
	bool AddItemToInventory(class UItemDataAsset* InItemDataAsset);
	void UseItemFromInventory(int8 InSlotIndex);

	// 인벤토리에 들어있는 골드 양 확인
	inline int32 GetGold() const { return Gold; }

	// 인벤토리에 골드 추가
	inline void AddGold(int32 Amount) { SetGold(Gold + Amount); }

	// 골드 변환를 알리는 델리게이트
	FOnPlayerGoldChange OnGoldChange;


	UFUNCTION(BlueprintCallable)
	inline void TestUseItem(int32 InSlotIndex)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Use item in slot %d"), InSlotIndex));
		UseItemFromInventory(InSlotIndex);
	}

	UFUNCTION(BlueprintCallable)
	inline void TestPrintInventory(){ Inven.TestPrintInventory(); }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
	// 골드에 변화가 있을 때 데이터 변화와 델리게이트 발동을 한번에 처리하기 위한 세터
	inline void SetGold(int32 NewGold) { Gold = NewGold; OnGoldChange.Broadcast(Gold); }

	UFUNCTION()
	inline void TestGoldChange(int32 NewGold)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Gold changed to %d"), NewGold));
	}

public:	

protected:
	// 인벤토리에 들어있는 골드 양
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gold")
	int32 Gold = 0;

private:
	// 인벤토리 객체
	Inventory Inven;
		
};
