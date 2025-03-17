// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/UniformGridPanel.h"
#include "ItemSlotWidget.h"
#include "Action/Player/ActionPlayerState.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/CanvasPanelSlot.h"
#include "GameFramework/GameUserSettings.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!Canvas)
	{
		Canvas = NewObject<UCanvasPanel>();
	}

	if (!ItemSlotsGrid)
	{
		ItemSlotsGrid = NewObject<UUniformGridPanel>();
		Canvas->AddChild(ItemSlotsGrid);
	}

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerState = PlayerController->GetPlayerState<AActionPlayerState>();	

			if (PlayerState)
			{
				int32 Count = ItemSlotsGrid->GetChildrenCount();
				for (int32 i = 0; i < Count; i++)
				{
					UItemSlotWidget* ItemSlotWidget = Cast<UItemSlotWidget>(ItemSlotsGrid->GetChildAt(i));
					if (ItemSlotWidget)
					{
						EInvenSlotType SlotType = static_cast<EInvenSlotType>(i);
						ItemSlotWidget->InitializeItemSlot(i, PlayerState->GetInvenSlot(SlotType));
						ItemSlotWidget->OnSlotClicked.AddDynamic(this, &UInventoryWidget::OnSlotClicked);
						ItemSlotWidgets.Add(ItemSlotWidget);
					}
				}

				TempSlotWidget->InitializeItemSlot(static_cast<int32>(EInvenSlotType::Temporary),
					PlayerState->GetInvenSlot(EInvenSlotType::Temporary));
				
			}
		}
	}
}

void UInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	float DownScale = 720.0f / 1080.0f;

	if (TempSlotWidget)
	{
		FVector2D StartPosition;	// 인벤토리 왼쪽 위 기준
		

		FVector2D MousePosition;	// 실제 마우스 좌표
		FVector2D TargetPosition;	// 해상도 기준으로 변경한 좌표
		if (UWidgetLayoutLibrary::GetMousePositionScaledByDPI(
			GetWorld()->GetFirstPlayerController(), MousePosition.X, MousePosition.Y))	// 해상도 기준 마우스 위치
		{
			UCanvasPanelSlot* ParentCanvasSlot = Cast<UCanvasPanelSlot>(this->Slot);
			FVector2D ParentPosition = ParentCanvasSlot->GetPosition();

			FAnchors ParentAnchors = ParentCanvasSlot->GetAnchors();
			FVector2D ParentAlignment = ParentCanvasSlot->GetAlignment();

			FVector2D ParentSize = ParentCanvasSlot->GetSize();
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("ParentPosition: %s"), *ParentPosition.ToString()));

			FVector2D ViewportSize;
			//GEngine->GameViewport->GetViewportSize(ViewportSize);
			ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("ViewportSize: %s"), *ViewportSize.ToString()));


			StartPosition = ViewportSize + (ParentPosition - ParentSize) * DownScale;
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("StartPosition: %s"), *StartPosition.ToString()));

			GEngine->GameViewport->GetMousePosition(MousePosition);		// 뷰포트 기준 마우스 위치
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("MousePosition: %s"), *MousePosition.ToString()));

			FVector2D Diff = MousePosition - StartPosition;
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Diff: %s"), *Diff.ToString()));

			//FVector2D LocalMousePosition = MyGeometry.AbsoluteToLocal(MousePosition);
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("LocalMousePosition: %s"), *LocalMousePosition.ToString()));



			UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();
			FVector2D ScreenResolution;
			if (UserSettings)
			{
				ScreenResolution = UserSettings->GetScreenResolution();

				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Resolution: %s"), *ScreenResolution.ToString()));
			}

			TargetPosition = (ScreenResolution / ViewportSize)* MousePosition;
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("(ScreenResolution / ViewportSize): %s"), *(ScreenResolution / ViewportSize).ToString()));
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("TargetPosition: %s"), *TargetPosition.ToString()));
			

			UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(TempSlotWidget->Slot);
			if (CanvasSlot)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("MousePosition: %s"), *MousePosition.ToString()));
				//CanvasSlot->SetPosition(MousePosition - ParentPosition);	// 부모에 대한 상대 좌표로 설정
				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("RelativePosition: %s"), *(TargetPosition - ScreenResolution).ToString()));

				//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Final : %s"), *(TargetPosition - ScreenResolution + ParentSize - ParentPosition).ToString()));
				//CanvasSlot->SetPosition(TargetPosition - ScreenResolution + ParentSize - ParentPosition);	// 부모에 대한 상대 좌표로 설정

				CanvasSlot->SetPosition(FVector2D(1920 * 0.5f, 1080 * 0.5f));	// 파라메터는 해상도 기준으로 들어가는데 실제 위치는 뷰포트 기준으로 다운스케일되서 설정됨

				//CanvasSlot->SetPosition(ParentPosition + TargetPosition);
			}
		}
	}

	// 마우스 위치는 뷰포트 기준(실제 해상도)
	// 새로 놓는 위치도 뷰포트 기준(실제 해상도)?
	// UI 위치 : 설정된 해상도 기준(1080p)
	// UI 크기 : 설정된 해상도 기준(1080p)


	// x가 비율을 맞추려는 느낌이다.
}

void UInventoryWidget::RefreshInventory()
{
	for (auto& ItemSlotWidget : ItemSlotWidgets)
	{
		ItemSlotWidget->RefreshSlot();
	}
}

void UInventoryWidget::OnSlotClicked(int32 InSlotIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("Inventory : Slot(%d) Clicked"), InSlotIndex);

	if (TempSlotWidget->IsEmpty())
	{
		// 임시 슬롯이 비어있으면 InSlotIndex슬롯에서 임시 슬롯으로 아이템 이동
		PlayerState->MoveItemFromInventory(static_cast<EInvenSlotType>(InSlotIndex), EInvenSlotType::Temporary);
	}
	else
	{
		// 임시 슬롯이 차있으면 InSlotIndex슬롯과 임시 슬롯 아이템 교환
		PlayerState->MoveItemFromInventory(EInvenSlotType::Temporary, static_cast<EInvenSlotType>(InSlotIndex));
	}	
}