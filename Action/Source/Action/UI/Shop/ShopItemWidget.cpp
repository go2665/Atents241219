// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopItemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Action/Item/ItemData/ItemDataAsset.h"
#include "Action/Player/ActionPlayerState.h"

void UShopItemWidget::SetItemDataAsset(UItemDataAsset* NewItemDataAsset)
{
	ItemDataAsset = NewItemDataAsset;
	if (ItemDataAsset)
	{
		ItemIcon->SetBrushFromTexture(ItemDataAsset->ItemIcon);
		ItemName->SetText(ItemDataAsset->ItemName);
		ItemPrice->SetText(FText::AsNumber(ItemDataAsset->ItemPrice));
		ItemDescription->SetText(ItemDataAsset->ItemDescription);
	}
	else
	{
		ItemIcon->SetBrushFromTexture(nullptr);
		ItemIcon->SetColorAndOpacity(FLinearColor::Transparent);
		ItemName->SetText(FText::FromName(TEXT("")));
		ItemPrice->SetText(FText::AsNumber(0));
		ItemDescription->SetText(FText::FromName(TEXT("")));
	}

	ItemBuy->SetIsEnabled(PlayerState->CanBuyItem(ItemDataAsset, MinimumItemCount));
}

void UShopItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	PlayerState = PlayerController->GetPlayerState<AActionPlayerState>();

	ItemCount->HintText = FText::AsNumber(MinimumItemCount);
	ItemCount->OnTextChanged.AddDynamic(this, &UShopItemWidget::OnItemCountTextChanged);
	ItemCount->OnTextCommitted.AddDynamic(this, &UShopItemWidget::OnItemCountTextCommitted);
	
	//ItemCount->OnTextChanged
	// 버튼에 함수 바인딩
}

void UShopItemWidget::OnItemCountTextChanged(const FText& InText)
{
	// ItemCount에 숫자만 입력되도록 설정
	// 0보다 커야 한다.
	// ItemCount만큼 구매가 불가능할 경우 구매 버튼을 비활성화한다.(돈이 부족하거나 인벤토리 공간이 부족할 경우)

	if (ItemDataAsset)
	{
		FString str = InText.ToString();
		if (str.IsNumeric())
		{
			int32 Count = FCString::Atoi(*str);
			if (Count < MinimumItemCount)
			{
				Count = MinimumItemCount;
			}
			ItemCount->SetText(FText::AsNumber(Count));

			// 구매 버튼 활성화 여부 결정
			ItemBuy->SetIsEnabled(PlayerState->CanBuyItem(ItemDataAsset, Count));
		}
	}	
}

void UShopItemWidget::OnItemCountTextCommitted(const FText& InText, ETextCommit::Type CommitMethod)
{
	FString str = InText.ToString();
	if (!str.IsNumeric())
	{
		ItemCount->SetText(FText::AsNumber(MinimumItemCount));
	}
}
