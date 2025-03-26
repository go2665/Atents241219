// Fill out your copyright notice in the Description page of Project Settings.


#include "MazeExitActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"


// Sets default values
AMazeExitActor::AMazeExitActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	Box->SetupAttachment(Root);	

	Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effect"));
	Effect->SetupAttachment(Root);
	Effect->SetAutoActivate(false);
}

void AMazeExitActor::ActivateExit(ACellActor* InClearActor)
{
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Effect->Activate();
}

// Called when the game starts or when spawned
void AMazeExitActor::BeginPlay()
{
	Super::BeginPlay();
	
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	OnActorBeginOverlap.AddDynamic(this, &AMazeExitActor::OnMazeExitBeginOverlap);
}

void AMazeExitActor::OnMazeExitBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("Player"))
	{
		UE_LOG(LogTemp, Warning, TEXT("Game Clear"));
		OnGameClear.Broadcast();
	}
	
}

