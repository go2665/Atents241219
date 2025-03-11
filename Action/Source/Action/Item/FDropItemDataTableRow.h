#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EItemType.h"
#include "FDropItemDataTableRow.generated.h"

USTRUCT(BlueprintType)
struct FDropItemDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType = EItemType::GoldCoin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Chance = 0.0f;
};
