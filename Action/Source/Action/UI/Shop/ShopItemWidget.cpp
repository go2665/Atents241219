// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopItemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Action/Item/ItemData/ItemDataAsset.h"
#include "Action/Player/ActionPlayerState.h"

void UShopItemWidget::SetItemDataAsset(UItemDataAsset* NewItemDataAsset)
{
	// 아이뎀 데이터 세팅
	ItemDataAsset = NewItemDataAsset;
	if (ItemDataAsset)
	{
		// UI 위젯들 표시 변경
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

	// 구매 버튼 활성화 여부 결정
	ItemBuy->SetIsEnabled(PlayerState->CanBuyItem(ItemDataAsset, MinimumItemCount));
}

void UShopItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	PlayerState = PlayerController->GetPlayerState<AActionPlayerState>();

	if (ItemCount)
	{
		ItemCount->HintText = FText::AsNumber(MinimumItemCount);
		ItemCount->OnTextChanged.AddDynamic(this, &UShopItemWidget::OnItemCountTextChanged);		// 글자가 변경되었을 때
		ItemCount->OnTextCommitted.AddDynamic(this, &UShopItemWidget::OnItemCountTextCommitted);	// 글자가 확정되었을 때
	}
	
	if (ItemBuy)
	{
		// 버튼에 함수 바인딩
		ItemBuy->OnClicked.AddDynamic(this, &UShopItemWidget::OnBuyButtonClicked);
	}
}

void UShopItemWidget::OnItemCountTextChanged(const FText& InText)
{
	if (ItemDataAsset)
	{
		FString str = InText.ToString();
		if (str.IsNumeric())
		{
			// 문자열에 숫자만 입력되었을 때 처리
			int32 Count = FCString::Atoi(*str);	// 최소 구매 개수 이하로 내려가지 않도록 처리
			if (Count < MinimumItemCount)
			{
				Count = MinimumItemCount;
			}
			ItemCount->SetText(FText::AsNumber(Count));

			// 변화된 개수에 따라 구매 버튼 활성화 여부 결정
			ItemBuy->SetIsEnabled(PlayerState->CanBuyItem(ItemDataAsset, Count));
		}
	}	
}

void UShopItemWidget::OnItemCountTextCommitted(const FText& InText, ETextCommit::Type CommitMethod)
{
	FString str = InText.ToString();
	if (!str.IsNumeric())
	{
		// 숫자가 아닌 문자열이 입력되었을 때 무조건 최소 구매 개수로 변경
		ItemCount->SetText(FText::AsNumber(MinimumItemCount));
	}
}

void UShopItemWidget::OnBuyButtonClicked()
{
	FString str;
	if (ItemCount->GetText().IsEmptyOrWhitespace())
	{
		str = ItemCount->GetHintText().ToString();	// 입력된 값이 없으면 힌트 텍스트로 대체
	}
	else
	{
		str = ItemCount->GetText().ToString();
	}

	int32 Count = FCString::Atoi(*str);
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, 
	//	FString::Printf(TEXT("Item Count : %d"), Count));

	int32 TotalPrice = ItemDataAsset->ItemPrice * Count;	// 최종 금액 계산

	PlayerState->AddGold(-TotalPrice);	// 골드 차감
	
	for (int i = 0; i < Count; i++)
	{
		PlayerState->AddItemToInventory(ItemDataAsset);		// 구매한 수만큼 인벤토리에 아이템 추가
	}
}
