// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EItemType.h"
#include "DropItemFactoryComponent.generated.h"

USTRUCT(BlueprintType)
struct FDropItemData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class ADropItemBase> DropItemClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UItemDataAsset* ItemDataAsset = nullptr;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTION_API UDropItemFactoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDropItemFactoryComponent();

	UFUNCTION(BlueprintCallable, Category = "DropItem")
	class ADropItemBase* GetDropItem(
		EItemType InItemType, 
		FVector InLocation = FVector::ZeroVector, FRotator InRotation = FRotator::ZeroRotator);

	UFUNCTION(BlueprintCallable, Category = "DropItem")
	inline class UItemDataAsset* GetItemDataAsset(EItemType InItemType) const {
		return DropItemDataMap.Contains(InItemType) ? DropItemDataMap[InItemType].ItemDataAsset : nullptr;	
	};

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DropItem Data")
	TMap<EItemType, FDropItemData> DropItemDataMap;
};
