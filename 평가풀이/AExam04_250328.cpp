// Fill out your copyright notice in the Description page of Project Settings.


#include "AExam04_250328.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "FWidgetSettingDataTableRow.h"

void UAExam04_250328::NativeConstruct()
{
	Super::NativeConstruct();

	if (DataTable)
	{
		InitializeData();
	}

	if(Button1)
	{
		Button1->OnClicked.AddDynamic(this, &UAExam04_250328::OnButton1Clicked);
	}

	if (Button2)
	{
		Button2->OnClicked.AddDynamic(this, &UAExam04_250328::OnButton2Clicked);
	}

	if (Button3)
	{
		Button3->OnClicked.AddDynamic(this, &UAExam04_250328::OnButton3Clicked);
	}
}

void UAExam04_250328::InitializeData()
{
	RowNames.Empty();
	RowNames = DataTable->GetRowNames();
	RowDatas.Empty();
	DataTable->GetAllRows<FWidgetSettingDataTableRow>(TEXT(""), RowDatas);
	//for (auto& RowName : RowNames)
	//{
	//	FWidgetSettingDataTableRow* RowData = DataTable->FindRow<FWidgetSettingDataTableRow>(RowName, TEXT(""));
	//	if (RowData)
	//	{
	//		RowDatas.Add(RowData);
	//	}
	//}
}

void UAExam04_250328::OnButton1Clicked()
{
	//FWidgetSettingDataTableRow* RowData = DataTable->FindRow<FWidgetSettingDataTableRow>(RowNames[0], TEXT(""));
	//SetWidgetSetting(RowNames[0], RowData);

	SetWidgetSetting(RowNames[0], RowDatas[0]);
}

void UAExam04_250328::OnButton2Clicked()
{
	SetWidgetSetting(RowNames[1], RowDatas[1]);
}

void UAExam04_250328::OnButton3Clicked()
{
	SetWidgetSetting(RowNames[2], RowDatas[2]);
}

void UAExam04_250328::SetWidgetSetting(FName RowName, FWidgetSettingDataTableRow* RowData)
{
	if (RowData && !RowName.IsNone())
	{
		TextBlock->SetText(FText::FromName(RowName));
		Image->SetBrushFromTexture(RowData->Texture);

		UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(Image->Slot);
		if (CanvasSlot)
		{
			CanvasSlot->SetAnchors(
				FAnchors(RowData->AnchorAndAlign.X, RowData->AnchorAndAlign.Y, 
					RowData->AnchorAndAlign.X, RowData->AnchorAndAlign.Y));
			CanvasSlot->SetAlignment(FVector2D(RowData->AnchorAndAlign.X, RowData->AnchorAndAlign.Y));
			CanvasSlot->SetPosition(FVector2D(RowData->Position.X, RowData->Position.Y));
			CanvasSlot->SetSize(FVector2D(RowData->Size.X, RowData->Size.Y));
		}
		
	}
}
