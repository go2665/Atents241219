// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItemFactoryComponent.h"
#include "./DropItem/DropItemBase.h"
#include "./ItemData/ItemDataAsset.h"


// Sets default values for this component's properties
UDropItemFactoryComponent::UDropItemFactoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

ADropItemBase* UDropItemFactoryComponent::GetDropItem(EItemType InItemType, FVector InLocation, FRotator InRotation)
{
	ADropItemBase* Item = nullptr;

	if (DropItemDataMap.Contains(InItemType))	// 키가 존재하는지 확인
	{
		UWorld* World = GetWorld();
		const FDropItemData* DropItemData = DropItemDataMap.Find(InItemType);	// 키로 값 찾기

		if (World && DropItemData && DropItemData->DropItemClass)	// 월드와 데이터가 존재하는지 확인
		{
			Item = World->SpawnActor<ADropItemBase>(
				DropItemData->DropItemClass, InLocation, InRotation);	// 아이템 스폰

			if (Item)
			{
				Item->InitializeItemDataAsset(DropItemData->ItemDataAsset);	// 아이템 데이터 초기화
			}
		}
	}

	return Item;
}

// Called when the game starts
void UDropItemFactoryComponent::BeginPlay()
{
	Super::BeginPlay();	
}



