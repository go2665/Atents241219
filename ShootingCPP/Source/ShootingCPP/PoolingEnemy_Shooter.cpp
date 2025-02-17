// Fill out your copyright notice in the Description page of Project Settings.


#include "PoolingEnemy_Shooter.h"

APoolingEnemy_Shooter::APoolingEnemy_Shooter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APoolingEnemy_Shooter::OnActivate()
{
	Super::OnActivate();
	EnterMoveState();
}

void APoolingEnemy_Shooter::EnterMoveState()
{
	ProjectileMovement->Velocity = MoveSpeed * GetActorForwardVector(); // 앞으로 이동하게 설정
	FTimerManager& TimerManager = GetWorldTimerManager();
	FTimerHandle TempHandle;
	TimerManager.SetTimer(
		TempHandle,
		this, &APoolingEnemy_Shooter::EnterLookAtState,
		MoveDuration);	// MoveDuration시간 후에 EnterLookAtState 함수 호출
}

void APoolingEnemy_Shooter::EnterLookAtState()
{
	ProjectileMovement->Velocity = FVector::ZeroVector;	// 이동 중지
	UWorld* World = GetWorld();
	APlayerController* PlayerController = World->GetFirstPlayerController();
	APawn* PlayerPawn = PlayerController->GetPawn();
	if (PlayerPawn)	// 컨트롤러가 조종하는 폰은 없을 수도 있음
	{
		LookAtLocation = PlayerPawn->GetActorLocation();	// 플레이어 위치를 목표로 설정

		FTimerManager& TimerManager = World->GetTimerManager();
		TimerManager.SetTimer(
			LookAtTickTimer,
			this, &APoolingEnemy_Shooter::TurnToLookAtLocation,
			TurnTickInterval, true);	// 0.03초 마다 TurnToLookAtLocation 함수 호출
	}
}

void APoolingEnemy_Shooter::EnterFireState()
{
	CurrentFireCount = 0;	// 현재 발사 횟수 초기화
	FTimerManager& TimerManager = GetWorldTimerManager();
	TimerManager.SetTimer(
		FireIntervalTimer,
		this, &APoolingEnemy_Shooter::FireOneShot,
		FireInterval, true);	// FireInterval마다 FireOneShot 함수 호출
}

void APoolingEnemy_Shooter::TurnToLookAtLocation()
{
	FRotator CurrentRot = GetActorRotation();						// 현재 회전 구하기
	FVector LookAtDirection = LookAtLocation - GetActorLocation();	// 목표를 바라보는 방향 구하기
	// FRotationMatrix::MakeFromX(LookAtDirection) : LookAtDirection이 forward가 되도록 하는 회전 메트릭스 만들기	
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookAtDirection).Rotator();		// 목표 회전 구하기
	FRotator NewRot = FMath::RInterpTo(CurrentRot, TargetRot, TurnTickInterval, TurnSpeed);	// 현재 회전에서 목표 회전으로 보간하기
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, NewRot.ToString());	// 보간된 회전값 출력

	SetActorRotation(NewRot);	// 보간된 회전으로 회전 설정
	if (IsTurnComplete())		// 회전이 완료되었으면
	{
		GetWorldTimerManager().ClearTimer(LookAtTickTimer);	// 타이머 해제
		LookAtTickTimer.Invalidate();						// 타이머 무효화
		EnterFireState();									// EnterFireState 함수 호출
	}
}

void APoolingEnemy_Shooter::FireOneShot()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("FireOneShot"));	// 나중에 총알 생성 코드로 대체
	CurrentFireCount++;	// 현재 발사 횟수 증가
	if (CurrentFireCount >= FireRepeatCount)	// 현재 발사 횟수가 FireRepeatCount보다 크거나 같아지면
	{
		FTimerManager& TimerManager = GetWorldTimerManager();
		TimerManager.ClearTimer(FireIntervalTimer);				// 발사 타이머 해제
		FireIntervalTimer.Invalidate();							// 발사 타이머 무효화

		FTimerHandle TempHandle;
		TimerManager.SetTimer(
			TempHandle,
			this, &APoolingEnemy_Shooter::EnterMoveState,
			MoveDuration);	// MoveDuration시간 후에 EnterMoveState 함수로 돌아가기
	}
}

bool APoolingEnemy_Shooter::IsTurnComplete() const
{
	FVector Direction = (LookAtLocation - GetActorLocation()).GetSafeNormal();	// 목표를 바라보는 방향 구하기
	float Scalar = FVector::DotProduct(Direction, GetActorForwardVector());		// Direction과 액터의 Forward 벡터의 내적 구하기
	float Radian = FMath::Acos(Scalar);				// 내적을 이용해서 두 벡터의 사이각 구하기(라디안)
	float Degree = FMath::RadiansToDegrees(Radian);	// 라디안을 각도로 변환

	return (Degree < 0.01f);	// 각도가 0.01도 미만이면 회전 완료로 판단해서 true 리턴. 아니면 false 리턴
}
