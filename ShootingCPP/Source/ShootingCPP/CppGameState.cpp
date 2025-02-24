// Fill out your copyright notice in the Description page of Project Settings.


#include "CppGameState.h"

void ACppGameState::AddScore(int32 Point)
{
	Score += Point;
	OnScoreChanged.Broadcast(Score);	// 점수 갱신하고 알리기
}

void ACppGameState::BeginPlay()
{
	Super::BeginPlay();
	OnScoreChanged.Broadcast(Score);	// 큰 의미는 없지만 그래도 초기화 한다는 느낌으로 실행
}
