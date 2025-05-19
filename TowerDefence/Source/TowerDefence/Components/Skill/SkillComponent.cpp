// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillComponent.h"
#include "Engine/OverlapResult.h" 
#include "TowerDefence/Player/PlayerSpectatorPawn.h"
#include "TowerDefence/Tower/Tower.h"
#include "TowerDefence/Framework/TowerDefencePlayerController.h"
#include "NiagaraFunctionLibrary.h"

USkillComponent::USkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;	// 쿨타임을 위해 틱을 활성화
}

void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld(); // 월드 가져오기
	APlayerController* PlayerController = World->GetFirstPlayerController(); // 플레이어 컨트롤러 가져오기
	APawn* Pawn = PlayerController->GetPawn(); // 플레이어의 Pawn을 가져온다.
	PlayerPawn = Cast<APlayerSpectatorPawn>(Pawn);

	ATowerDefencePlayerController* TowerDefencePlayerController = Cast<ATowerDefencePlayerController>(PlayerController); // 플레이어 컨트롤러를 ATowerDefencePlayerController로 캐스팅
	if (TowerDefencePlayerController)
	{
		TowerDefencePlayerController->OnMouseClickInput.AddUObject(this, &USkillComponent::OnSkillAreaClicked); // 마우스 클릭 이벤트 바인딩
	}

	ensure(SkillData);	// 스킬 데이터가 nullptr이 아닌지 확인

    ATower* HeroTower = Cast<ATower>(GetOwner());
	HeroTower->OnTowerLevelUp.AddLambda([this](int32 InLevel)
		{
			// 타워 레벨 업데이트
			TowerLevel = FMath::Min(InLevel, SkillData->LevelData.Num() - 1);				
			SkillLevelData = &SkillData->LevelData[TowerLevel];
		});
	TowerLevel = 0;	// 타워 레벨 초기화
	SkillLevelData = &SkillData->LevelData[TowerLevel]; 
}

void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SkillCoolTime -= DeltaTime; // 쿨타임 감소
}

void USkillComponent::ActivateSkill()
{
	if (SkillCoolTime < 0.0f) // 쿨타임이 끝났을 때만 스킬 사용 가능
	{
		bIsSkillActivated = true;
		PlayerPawn->AreaIndicatorActivate(); // AreaIndicator 활성화
	}
}

void USkillComponent::UseSkill(FVector InLocation)
{
    // 스킬 사용
    UE_LOG(LogTemp, Warning, TEXT("[%s] : [%s] Skill (Radius %.1f)"), 
        *GetOwner()->GetActorNameOrLabel(), 
		*SkillData->SkillName,
        SkillLevelData->Radius);

	TArray<TScriptInterface<IEffectTargetable>> EffectTargets;	// 스킬 적용 대상 배열
    FindActorsInRadius(InLocation, SkillLevelData->Radius, EffectTargets);

	if (SkillData->AoeData)
	{
		// SkillData->AoeData->AoeClass 스폰
		UWorld* World = GetWorld();
		AAreaOfEffect* AoeActor = World->SpawnActor<AAreaOfEffect>(
			SkillData->AoeData->AoeClass, InLocation, FRotator::ZeroRotator);

		AoeActor->OnInitialize(*SkillData, TowerLevel); // AOE 초기화		
	}

	for (TScriptInterface<IEffectTargetable> Target : EffectTargets)
	{
		if (Target->GetEffectTarget() == EEffectTarget::Hostile)		// 적대적인 대상이고
		{
			// 디버프 적용
			if (SkillData->DebuffType != EEffectType::None)				// 디버프가 있으면
			{
				Target->AddEffect(SkillData->DebuffType, TowerLevel); 	// 디버프 적용					
			}
		}
		else if (Target->GetEffectTarget() == EEffectTarget::Friendly)	// 아군 대상이고
		{
			// 버프 적용
			if (SkillData->BuffType != EEffectType::None)				// 버프가 있으면 
			{
				Target->AddEffect(SkillData->BuffType, TowerLevel);		// 버프 적용
			}
		}		
	}

    SkillCoolTime = SkillLevelData->CooldownTime;	// 쿨타임 초기화
    bIsSkillActivated = false;						// 스킬 사용 후 위치 선택 종료
}

void USkillComponent::OnSkillAreaClicked()
{
	//UE_LOG(LogTemp, Warning, TEXT("USkillComponent : Mouse Clicked!"));

	// 스킬 사용 중일 때 클릭이 일어났으면 그 위치(AreaIndicator 위치)에 스킬 사용
	if (bIsSkillActivated)
	{
		UseSkill(PlayerPawn->GetAreaIndicatorLocation()); // AreaIndicator 위치 가져와서 스킬 사용
	}
}

void USkillComponent::FindActorsInRadius(FVector Center, float Radius,  
   TArray<TScriptInterface<IEffectTargetable>>& OutEffectTargets)  
{  
   // 출력 파라메터 초기화  
   OutEffectTargets.Empty();  

   // 결과를 저장할 배열    
   TArray<FOverlapResult> OverlapResults;  

   // 적과 타워의 충돌 채널을 설정  
   FCollisionObjectQueryParams ObjectQueryParams;  
   ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel1);  // 적  
   ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_GameTraceChannel2);  // 타워  

   // 월드에서 Overlap 검사    
   bool bHasOverlaps = GetWorld()->OverlapMultiByObjectType(  
       OverlapResults,  
       Center,  
       FQuat::Identity,  
       ObjectQueryParams,  
       FCollisionShape::MakeCapsule(Radius, 200.0f) // 200.0f은 타워 높이가 150 정도이므로 여유분을 주기 위해 설정  
   );  

   // 오버랩된 결과가 있으면 OutEffectTargets에 저장  
   if (bHasOverlaps)  
   {  
       for (const FOverlapResult& Result : OverlapResults)  
       {  
           if (AActor* Actor = Result.GetActor())  
           {                  
               if (Actor->GetClass()->ImplementsInterface(UEffectTargetable::StaticClass()))  
               {  
                   TScriptInterface<IEffectTargetable> EffectTarget(Actor);
                   OutEffectTargets.Add(EffectTarget);  
               }  
           }  
       }  
   }  

   // 디버그용으로 구체를 그려줌 (선택 사항)    
   DrawDebugCapsule(GetWorld(), Center, 200.0f, Radius, FQuat::Identity, FColor::Green, false, 2.0f);  
}

