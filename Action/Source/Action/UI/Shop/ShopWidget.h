// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
};

// 상점 UI
// 1. 열리고 닫힐 수 있다.
//	1.1. 열리면 상점 아이템들을 보여준다.(ShopItemWidget들 채우기)
//  1.2. 상점 아이템 데이터 테이블을 이용해서 아이템 목록을 가져온다.
//  1.3. 데이터 테이블은 TArray로 여러개 가지고 있다가 그 중 하나를 랜덤으로 선택.