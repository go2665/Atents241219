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
	UFUNCTION(BlueprintCallable)
	inline bool AddItemToInventory(class UItemDataAsset* InItemDataAsset) { 
		return Inven.AddItem(InItemDataAsset);	/*인벤토리에 있는 아이템 추가 함수 실행*/ 
	};

	// 인벤토리에 있는 아이템을 옮기는 함수
	UFUNCTION(BlueprintCallable)
	inline void MoveItemFromInventory(EInvenSlotType InFromSlot, EInvenSlotType InToSlot) {
		Inven.MoveItem(InFromSlot, InToSlot);
	}

	// 인벤토리에 있는 아이템 사용
	UFUNCTION(BlueprintCallable)
	inline void UseItemFromInventory(uint8 InSlotIndex) {
		Inven.UseItem(InSlotIndex);	// 인벤토리에 있는 아이템 사용 함수 실행
	}

	// 인벤토리에 있는 무기 장비
	UFUNCTION(BlueprintCallable)
	inline void EquipItemFromInventory(uint8 InSlotIndex) {
		Inven.EquipItem(InSlotIndex);	// 인벤토리에 있는 무기 장비 함수 실행
	}

	// 인벤토리에 있는 슬롯 주소 리턴
	inline InvenSlotBase* GetInvenSlot(EInvenSlotType InSlotType) { return Inven.GetInvenSlot(InSlotType); }

	// 인벤토리에 들어있는 골드 양 확인
	inline int32 GetGold() const { return Gold; }

	// 인벤토리에 골드 추가
	inline void AddGold(int32 Amount) { SetGold(Gold + Amount); }

	// 골드 변환를 알리는 델리게이트
	FOnPlayerGoldChange OnGoldChange;

	// 테스트용 함수
	UFUNCTION(BlueprintCallable)
	inline void TestPrintInventory(){ Inven.TestPrintInventory(); }

	UFUNCTION(BlueprintCallable)
	inline void TestInventoryAddDefaultItems(class UDataTable* TestTable = nullptr);

	UFUNCTION(BlueprintCallable)
	inline void TestItemEquip(EInvenSlotType InSlotType) { Inven.EquipItem(static_cast<uint8>(InSlotType)); };

	UFUNCTION(BlueprintCallable)
	inline void TestUseItem(int32 InSlotIndex) { Inven.UseItem(InSlotIndex); };

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
