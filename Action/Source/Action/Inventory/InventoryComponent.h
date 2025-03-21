// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EInvenSlotType.h"
#include "InvenSlot.h"
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

	// 인벤토리 기본 기능들 --------------------------------------------------------------------------------------

	// 인벤토리 초기화
	void InitInventory(class AActionPlayerCharacter* InOwner);

	// 인벤토리에 아이템 추가
	UFUNCTION(BlueprintCallable)
	bool AddItem(class UItemDataAsset* InItemDataAsset);

	// 인벤토리에 있는 아이템 옮기기
	UFUNCTION(BlueprintCallable)
	void MoveItem(EInvenSlotType InFromSlot, EInvenSlotType InToSlot);

	// 인벤토리의 특정 슬롯에 있는 아이템을 판매
	UFUNCTION(BlueprintCallable)
	int32 SellItem(EInvenSlotType InSlot);

	// 특정 슬롯에 있는 아이템 사용
	UFUNCTION(BlueprintCallable)
	void UseItem(EInvenSlotType InSlot);

	// 특정 슬롯에 있는 무기 아이템 장비
	UFUNCTION(BlueprintCallable)
	void EquipItem(EInvenSlotType InSlot);

	// 특정한 아이템을 특정 개수만큼 구매 가능한지 확인하는 함수
	UFUNCTION(BlueprintCallable)
	bool CanBuyItem(class UItemDataAsset* InItemDataAsset, int32 InItemCount);

	// 인벤토리에 골드 추가
	inline void AddGold(int32 Amount) { SetGold(Gold + Amount); }


	// Getter -----------------------------------------------------------------------------------------------

	// 인벤토리에 있는 특정 슬롯 주소 리턴
	inline UInvenSlot* GetInvenSlot(EInvenSlotType InSlotType) {
		if (InvenSlots.Contains(InSlotType)) 
			return InvenSlots[InSlotType]; 
		else 
			return nullptr;
	}

	// 인벤토리에 들어있는 골드 양 확인
	inline int32 GetGold() const { return Gold; }

public:
	// 골드 변환를 알리는 델리게이트
	FOnPlayerGoldChange OnGoldChange;

public:
	// 테스트용 함수 ------------------------------------------------------------------------------------------

	// 인벤토리 로그로 출력
	UFUNCTION(BlueprintCallable)
	void TestPrintInventory();

	// 인벤토리에 기본 아이템 추가
	UFUNCTION(BlueprintCallable)
	void TestInventoryAddDefaultItems(class UDataTable* TestTable = nullptr);

	// 인벤토리에 돈 추가
	UFUNCTION(BlueprintCallable)
	void TestAddGold(int32 Amount);

	// 골드 변화를 화면에 출력
	UFUNCTION()
	inline void TestPrintGoldChange(int32 NewGold) {
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Gold changed to %d"), NewGold));
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// 골드에 변화가 있을 때 데이터 변화와 델리게이트 발동을 한번에 처리하기 위한 세터
	inline void SetGold(int32 NewGold) { Gold = NewGold; OnGoldChange.Broadcast(Gold); }


public:	

protected:
	// 인벤토리에 들어있는 골드 양
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gold")
	int32 Gold = 0;

private:
	// 비어있는 슬롯 반환
	UInvenSlot* GetEmptySlot();

	// 비어있는 스롯에 아이템 추가
	bool AddItemToEmptySlot(class UItemDataAsset* InItemDataAsset);

	// 적절한 인덱스인지 확인하는 함수(가능한 사용하지 말 것. EInvenSlotType을 사용해 슬롯에 접근하는 것을 권장)
	inline bool IsValidIndex(uint8 InSlotIndex) { return InSlotIndex < MaxSlotCount; }

	// 최대 일반 슬롯 갯수
	static const int8 MaxSlotCount = 10;

	// 인벤토리 슬롯 맵
	UPROPERTY()				// UPROPERTY를 붙이지 않으면 UInvenSlot*가 가비지 콜랙팅의 대상이 된다.
	TMap<EInvenSlotType, UInvenSlot*> InvenSlots;

	// 인벤토리의 소유자(플레이어)
	class AActionPlayerCharacter* Owner = nullptr;
		
};
