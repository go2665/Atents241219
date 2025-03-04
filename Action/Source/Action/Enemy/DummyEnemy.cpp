// Fill out your copyright notice in the Description page of Project Settings.


#include "DummyEnemy.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "../UI/UserWidget_PopupDamage.h"

// Sets default values
ADummyEnemy::ADummyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 위젯 컴포넌트 생성
	PopupDamageWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("PopupDamageWidget"));
	PopupDamageWidget->SetupAttachment(RootComponent);
	PopupDamageWidget->SetWidgetSpace(EWidgetSpace::Screen);	// 스크린 스페이스로 그리기
}

// Called when the game starts or when spawned
void ADummyEnemy::BeginPlay()
{
	Super::BeginPlay();
	if (PopupDamageWidget)
	{
		// 데미지를 입었을 때 데미지 팝업을 띄우기 위한 델리게이트 바인딩
		PopupDamageWidgetInstance = Cast<UUserWidget_PopupDamage>(PopupDamageWidget->GetUserWidgetObject());
		OnTakeDamage.AddDynamic(PopupDamageWidgetInstance, &UUserWidget_PopupDamage::ActivateWidget);
	}
}

float ADummyEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("Damage : %f"), ActualDamage));

	// 데미지를 입었을 때 델리게이트 호출(데미지 팝업 띄우기)
	OnTakeDamage.Broadcast(ActualDamage);

	return ActualDamage;
}

// Called every frame
void ADummyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

// Called to bind functionality to input
void ADummyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}