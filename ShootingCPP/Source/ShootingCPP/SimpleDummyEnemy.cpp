// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleDummyEnemy.h"
#include "ProjectileDamageTypeBase.h"

// Sets default values
ASimpleDummyEnemy::ASimpleDummyEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// 루트 컴포넌트 생성
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);	// 루트 컴포넌트로 설정

	// 스테틱 메쉬 컴포넌트 생성
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Mesh->SetGenerateOverlapEvents(true);	// 겹침 이벤트를 받도록 설정

}

// Called when the game starts or when spawned
void ASimpleDummyEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;

	this->Tags.Add(FName("Enemy"));

	OnTakeAnyDamage.AddDynamic(this, &ASimpleDummyEnemy::OnAnyDamage);
}

// Called every frame
void ASimpleDummyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASimpleDummyEnemy::OnAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	float CurrentDamage = Damage;

	const UProjectileDamageTypeBase* ProjectileDamageType = Cast<UProjectileDamageTypeBase>(DamageType);
	if (ProjectileDamageType && ProjectileDamageType->Type == WeakType)
	{
		CurrentDamage *= 2.0f;
	}

	Health -= CurrentDamage;

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, 
		FString::Printf(TEXT("OnAnyDamage(%s) : %.1f, [%.1f/%.1f]"), 
			*(DamagedActor->GetActorLabel()), 
			CurrentDamage, Health, MaxHealth));
}

