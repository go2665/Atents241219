// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

/**
 * 아이템 정보 저장용 데이터 에셋
 */
UCLASS(BlueprintType)
class ACTION_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UItemDataAsset();

	// 스택 가능한 아이템인지 확인(가능하면 true, 아니면 false)
	inline bool IsStackable() const { return ItemStackCount > 1; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UStaticMesh* ItemMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UMaterialInterface* ItemMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	class UNiagaraSystem* HighlightEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (ClampMin = "0"))
	int32 ItemPrice = 0;

	// 인벤토리 한칸에 들어갈 수 있는 최대 아이템 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (ClampMin = "1"))
	int32 ItemStackCount = 1;
};
