// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopItemWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Action/Item/ItemData/ItemDataAsset.h"
#include "Action/Player/ActionPlayerState.h"

void UShopItemWidget::SetShopItemData(UItemDataAsset* InNewItemDataAsset, int32 InMaxCount)
{
	// 아이뎀 데이터 세팅
	ItemDataAsset = InNewItemDataAsset;
	StockCount = InMaxCount;
	if (ItemDataAsset)
	{
		// UI 위젯들 표시 변경
		ItemIcon->SetBrushFromTexture(ItemDataAsset->ItemIcon);
		ItemName->SetText(ItemDataAsset->ItemName);
		ItemPrice->SetText(FText::AsNumber(ItemDataAsset->ItemPrice));
		ItemDescription->SetText(ItemDataAsset->ItemDescription);
		ItemStockCount->SetText(FText::AsNumber(StockCount));
	}
	else
	{
		ItemIcon->SetBrushFromTexture(nullptr);
		ItemIcon->SetColorAndOpacity(FLinearColor::Transparent);
		ItemName->SetText(FText::FromName(TEXT("")));
		ItemPrice->SetText(FText::AsNumber(0));
		ItemDescription->SetText(FText::FromName(TEXT("")));
		ItemStockCount->SetText(FText::AsNumber(0));
	}

	// 구매 버튼 활성화 여부 결정
	ItemBuy->SetIsEnabled(PlayerState->CanBuyItem(ItemDataAsset, MinimumItemCount));
}

void UShopItemWidget::UpdateBuyButtonState()
{
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
		ItemCount->SetHintText(FText::AsNumber(MinimumItemCount));
		ItemCount->OnTextChanged.AddDynamic(this, &UShopItemWidget::OnItemCountTextChanged);		// 글자가 변경되었을 때
		ItemCount->OnTextCommitted.AddDynamic(this, &UShopItemWidget::OnItemCountTextCommitted);	// 글자가 확정되었을 때
	}
	
	if (ItemBuy)
	{
		// 버튼에 함수 바인딩
		ItemBuy->OnClicked.AddDynamic(this, &UShopItemWidget::OnBuyButtonClicked);
	}

	SoldOut->SetVisibility(ESlateVisibility::Hidden);
}

void UShopItemWidget::OnItemCountTextChanged(const FText& InText)
{
	if (ItemDataAsset)
	{
		FString str = InText.ToString();
		if (str.IsNumeric())
		{
			// 문자열에 숫자만 입력되었을 때 처리
			int32 Count = FCString::Atoi(*str);	// 최소 구매 개수보다 크거나 같고 재고 개수보다는 작거나 같아야한다.
			Count = FMath::Clamp(Count, MinimumItemCount, StockCount);
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
		ItemCount->SetText(FText::AsNumber(StockCount));
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

	StockCount -= Count;	// 재고 개수 감소
	ItemStockCount->SetText(FText::AsNumber(StockCount));	// 표시되는 재고 개수 갱신
	OnItemCountTextChanged(FText::AsNumber(FMath::Min(Count, StockCount)));	// Count가 StockCount를 넘지 않게 하기
	if (StockCount < 1)
	{
		OnSoldOut();		// 재고가 다 떨어졌을 때 실행
	}

	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow,
	//	FString::Printf(TEXT("Item StockCount : %d"), StockCount));
}

void UShopItemWidget::OnSoldOut()
{
	SoldOut->SetVisibility(ESlateVisibility::Visible);
	ItemBuy->SetIsEnabled(false);
	ItemCount->SetIsEnabled(false);
	ItemCount->SetText(FText::FromString(TEXT("-")));
}
