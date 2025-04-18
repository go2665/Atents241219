#include "HeroTowerActor.h"  
#include "Kismet/GameplayStatics.h"
#include "TowerDefence/Enemy/EnemyBase.h"  
#include "TowerDefence/Shot/Debuff/DebuffComponent.h"

void AHeroTowerActor::UseSkill(FVector InLocation)  
{  
   // 스킬 사용  
   USkillDataAsset* SkillDataAsset = SkillDataAssets[GunLevel - 1]; // 스킬 데이터 에셋을 가져온다.  
     
   //UE_LOG(LogTemp, Warning, TEXT("[%s] : Skill Radius %.1f"), *GetActorNameOrLabel(), SkillDataAsset->Radius);

   if (SkillDataAsset)  
   {  
       if (SkillDataAsset->TowerBuffDataAssets.Num() > 0)  
       {  
           // 타워를 찾아야 하는 경우
		   TArray<ATowerBaseActor*> Towers;
           FindActorsInRadius(InLocation, SkillDataAsset->Radius, Towers);

           // 버프 스킬  
		   for (ATowerBaseActor* Tower : Towers)
		   {
			   if (Tower)
			   {
				   // 버프 적용
				   for (UTowerBuffDataAsset* BuffDataAsset : SkillDataAsset->TowerBuffDataAssets)
				   {
					   Tower->AddBuff(BuffDataAsset);
				   }
			   }
		   }
       }  
       else
       {
		   // 적을 찾아야 하는 경우
           TArray<AEnemyBase*> Enemies;
           FindActorsInRadius(InLocation, SkillDataAsset->Radius, Enemies); // 반지름 안에 있는 적을 찾는다.

           if (SkillDataAsset->Damage > 0)
           {
               // 데미지 스킬(InLocation에서 반지름 Radius안에 있는 AEnemyBase*에 데미지 주기)  
               for (AEnemyBase* Enemy : Enemies)
               {
                   if (Enemy)
                   {
                       UGameplayStatics::ApplyDamage(
                           Enemy, SkillDataAsset->Damage,
                           nullptr, this, SkillDataAsset->DamageType);
                   }
               }
           }

           if (SkillDataAsset->DebuffTypes.Num() > 0)
           {
               // 디버프 스킬  
               for (AEnemyBase* Enemy : Enemies)
               {
                   for (EDebuffType DebuffType : SkillDataAsset->DebuffTypes)
                   {
                       Enemy->GetDebuffComponent()->AddDebuff(DebuffType, SkillDataAsset->DebuffModifier);
                   }
               }
           }
       }       
   }  
}  

//void AHeroTowerActor::FindEnemiesInRadius(FVector Center, float Radius, TArray<AEnemyBase*>& OutActors)  
//{  
//    // 출력 파라메터 초기화
//    OutActors.Empty();
//
//    // 결과를 저장할 배열  
//    TArray<FOverlapResult> OverlapResults;  
//
//    // 월드에서 Overlap 검사  
//    bool bHasOverlaps = GetWorld()->OverlapMultiByObjectType(  
//        OverlapResults,  
//        Center,  
//        FQuat::Identity,  
//        FCollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel1),   
//        FCollisionShape::MakeSphere(Radius)  
//    );  
//
//    // 결과 처리  
//    if (bHasOverlaps)  
//    {  
//        for (const FOverlapResult& Result : OverlapResults)  
//        {  
//            AEnemyBase* Enemy = Cast<AEnemyBase>(Result.GetActor());  
//            if (Enemy)  
//            {  
//                OutActors.Add(Enemy);  
//            }  
//        }  
//    }  
//
//    // 디버그용으로 구체를 그려줌 (선택 사항)  
//    DrawDebugSphere(GetWorld(), Center, Radius, 12, FColor::Green, false, 2.0f);  
//}
//
//void AHeroTowerActor::FindTowersInRadius(FVector Center, float Radius, TArray<ATowerBaseActor*>& OutActors)
//{
//    // 출력 파라메터 초기화
//    OutActors.Empty();
//
//    // 결과를 저장할 배열  
//    TArray<FOverlapResult> OverlapResults;
//
//    // 월드에서 Overlap 검사  
//    bool bHasOverlaps = GetWorld()->OverlapMultiByObjectType(
//        OverlapResults,
//        Center,
//        FQuat::Identity,
//        FCollisionObjectQueryParams(ECollisionChannel::ECC_GameTraceChannel2),
//        FCollisionShape::MakeSphere(Radius)
//    );
//
//    // 결과 처리  
//    if (bHasOverlaps)
//    {
//        for (const FOverlapResult& Result : OverlapResults)
//        {
//            ATowerBaseActor* Enemy = Cast<ATowerBaseActor>(Result.GetActor());
//            if (Enemy)
//            {
//                OutActors.Add(Enemy);
//            }
//        }
//    }
//
//    // 디버그용으로 구체를 그려줌 (선택 사항)  
//    DrawDebugSphere(GetWorld(), Center, Radius, 12, FColor::Green, false, 2.0f);
//}
