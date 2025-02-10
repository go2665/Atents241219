// Fill out your copyright notice in the Description page of Project Settings.


#include "TestPoolingObject.h"

ATestPoolingObject::ATestPoolingObject()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);
}

void ATestPoolingObject::DeactivateTest()
{
	Deactivate();
}

void ATestPoolingObject::OnActivate()
{
	Mesh->SetVisibility(true);
}

void ATestPoolingObject::OnDeactivate()
{
	Mesh->SetVisibility(false);
}
