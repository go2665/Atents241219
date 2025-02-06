// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWingController.h"

APlayerWingController::APlayerWingController()
{
	//bShowMouseCursor = true;
	SetShowMouseCursor(true);	// 변수로 세팅하는 것에 비해 로그만 추가로 찍어준다.
	DefaultMouseCursor = EMouseCursor::Crosshairs;	// 크로스헤어로 마우스 커서 바꾸기
													// (입력 초기화하면서 CurrentMouseCusor가 DefaultMouseCusor값으로 수정된다.)
	
}
