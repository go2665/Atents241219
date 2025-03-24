// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_DistanceCheck.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTDecorator_DistanceCheck::UBTDecorator_DistanceCheck()
{
	NodeName = "DistanceCheck";
}

void UBTDecorator_DistanceCheck::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	SquareDistanceThreshold = DistanceThreshold * DistanceThreshold;
}

bool UBTDecorator_DistanceCheck::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp)
	{
		AActor* TargetPlayer = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetPlayer"));
		APawn* ThisPawn = OwnerComp.GetAIOwner()->GetPawn();

		if (TargetPlayer && ThisPawn)
		{
			float SquaredDistance = FVector::DistSquaredXY(
				TargetPlayer->GetActorLocation(), ThisPawn->GetActorLocation());

			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, 
			//	FString::Printf(TEXT("Distance: %.1f, %.1f"), FMath::Sqrt(Distance), SquareDistanceThreshold));
			return SquaredDistance < SquareDistanceThreshold;
		}
	}

	return false;
}
