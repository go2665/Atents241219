// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_AreaDamage.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/ActionPlayerCharacter.h"

void UAnimNotify_AreaDamage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (PlayerCharacter == nullptr)
	{
		PlayerCharacter = Cast<AActionPlayerCharacter>(MeshComp->GetOwner());
	}

	if (PlayerCharacter)
	{
		// 중심 위치(플레이어 위치에서 CenterLength만큼 앞으로 나간 곳)
		FVector Center = PlayerCharacter->GetActorLocation() + PlayerCharacter->GetActorForwardVector() * CenterLength;
		
		DrawDebugSphere(
			MeshComp->GetWorld(),	// 월드
			Center,					// 중심 위치
			InnerRadius,			// 반지름
			12,						// 세그먼트 수	
			FColor::Red,			// 색상
			false,					// 영원히 지속할지 여부
			DebugDuration,			// 지속 시간
			0,						// 깊이 우선순위(0일수록 앞에 그려짐)
			1.0f);					// 두께

		DrawDebugSphere(
			MeshComp->GetWorld(),	// 월드
			Center,					// 중심 위치
			OuterRadius,			// 반지름
			12,						// 세그먼트 수	
			FColor::Yellow,			// 색상
			false,					// 영원히 지속할지 여부
			DebugDuration,			// 지속 시간
			0,						// 깊이 우선순위(0일수록 앞에 그려짐)
			1.0f);					// 두께

		TArray<AActor*> IgnoreActors = { PlayerCharacter };

		UGameplayStatics::ApplyRadialDamageWithFalloff(
			MeshComp->GetWorld(),
			PlayerCharacter->GetAreaDamage(),	// 기본 데미지
			1.0f,								// 최소 데미지
			Center,								// 중심 위치
			InnerRadius,						// 최대 데미지를 주는 범위
			OuterRadius,						// 거리에 따라 감쇠되는 데미지를 주는 범위
			FallOff,							// 감쇠 비율
			nullptr,							// 데미지 타입
			IgnoreActors,						// 데미지를 주지 않을 액터들
			nullptr,							// 데미지 주는 액터
			nullptr,							// 데미지 주는 컨트롤러
			ECollisionChannel::ECC_Pawn);		// 데미지를 주는 채널(폰만 대상으로 함)
	}
}
