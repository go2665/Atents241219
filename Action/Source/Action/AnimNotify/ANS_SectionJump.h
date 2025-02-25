// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_SectionJump.generated.h"

/**
 * 콤보 가능한 영역 표시 + 다음 콤보 섹션을 기록
 */
UCLASS()
class ACTION_API UANS_SectionJump : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	// 노티파이 시작시 호출
	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
		float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	// 노티파이가 끝날 때 호출
	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
		const FAnimNotifyEventReference& EventReference) override;

	inline FName GetNextSectionName() const { return NextSectionName; }

protected:
	// 다음 콤보용 섹션 이름
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SectionJump")
	FName NextSectionName;

private:
	// 이 애니메이션을 재생 시키고 있는 캐릭터
	UPROPERTY()
	class AActionPlayerCharacter* OwnerCharacter = nullptr;
};
