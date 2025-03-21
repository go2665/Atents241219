// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopWidget.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Action/Item/FShopItemDataTableRow.h"
#include "Action/Framework/ActionGameMode.h"
#include "Action/Player/ActionPlayerState.h"

void UShopWidget::Open()
{
	CurrentDataTable = GetRandomShopItemDataTable();	// 랜덤으로 데이터테이블 가져오기
	InitializeShopItemWidgets();							// 열기전에 판매할 아이템 목록	 갱신
	SetVisibility(ESlateVisibility::Visible);
}

void UShopWidget::Close()
{
	CurrentDataTable = nullptr;
	SetVisibility(ESlateVisibility::Hidden);
}

void UShopWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	AActionPlayerState* PlayerState = PlayerController->GetPlayerState<AActionPlayerState>();
	PlayerState->GetOnGoldChange().AddDynamic(this, &UShopWidget::OnMoneyChanged);	// 골드 변화 델리게이트에 함수 추가

	if (ShopItems)
	{
		int32 Count = ShopItems->GetChildrenCount();
		for (int32 i = 0; i < Count; i++)
		{
			UShopItemWidget* ShopItemWidget = Cast<UShopItemWidget>(ShopItems->GetChildAt(i));
			if (ShopItemWidget)
			{
				ShopItemWidgets.Add(ShopItemWidget);	// ShipItemWidget들 저장해 놓기
			}
		}
	}

	if (Exit)
	{
		Exit->OnClicked.AddDynamic(this, &UShopWidget::Close);	// 닫기 버튼에 함수 바인딩
	}
}

void UShopWidget::InitializeShopItemWidgets()
{
	if (IsOpen())	// 열려있을 때만 실행(CurrentDataTable있을 때만 실행)
	{
		UWorld* World = GetWorld();
		AActionGameMode* GameMode = World->GetAuthGameMode<AActionGameMode>();

		// 데이터테이블의 모든 행 가져와서 배열에 저장
		TArray<FShopItemDataTableRow*> Rows;
		CurrentDataTable->GetAllRows<FShopItemDataTableRow>(TEXT("InitializeShopItemWidgets"), Rows);	

		for (int32 i = 0; i < ShopItemWidgets.Num(); i++)
		{		
			// 순서대로 아이템 데이터 세팅
			ShopItemWidgets[i]->SetShopItemData(
				GameMode->GetItemDataAsset(Rows[i]->ItemType), Rows[i]->MaxCount);
		}
	}
}

void UShopWidget::UpdateBuyButtonsState()
{
	if (IsOpen())
	{
		for (UShopItemWidget* ShopItemWidget : ShopItemWidgets)
		{
			ShopItemWidget->UpdateBuyButtonState();	// 모든 상점 아이템 위젯의 구매 버튼 상태 갱신
		}
	}
}

UDataTable* UShopWidget::GetRandomShopItemDataTable()
{
	UDataTable* Result = nullptr;
	if (ShopItemDataTables.Num() > 0)
	{
		int32 Index = FMath::RandRange(0, ShopItemDataTables.Num() - 1);	// 모든 데이터테이블 중 랜덤으로 하나 선택
		Result = ShopItemDataTables[Index];
	}
	return Result;
}

void UShopWidget::OnMoneyChanged(int32 NewMoney)
{
	UpdateBuyButtonsState();	// 골드 변화가 있을 때마다 버튼 활성화 상태 변경
}
