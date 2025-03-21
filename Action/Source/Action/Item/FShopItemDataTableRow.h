#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EItemType.h"
#include "FShopItemDataTableRow.generated.h"

USTRUCT(BlueprintType)
struct FShopItemDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType = EItemType::Gem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "1"))
	int32 MaxCount = 1;
};
