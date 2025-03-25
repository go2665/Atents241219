// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Death.h"
#include "Action/Enemy/EnemyBase.h"

void UAnimNotify_Death::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// 적 캐릭터의 애니메이션 몽타주가 끝나면 PostDieAnimation 함수를 호출한다.
	AEnemyBase* Enemy = Cast<AEnemyBase>(MeshComp->GetOwner());
	if (Enemy)
	{
		Enemy->PostDieAnimation();
	}
}
