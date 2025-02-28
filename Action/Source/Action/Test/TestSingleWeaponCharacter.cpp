// Fill out your copyright notice in the Description page of Project Settings.


#include "TestSingleWeaponCharacter.h"
#include "../Weapon/WeaponActor.h"

void ATestSingleWeaponCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (TestWeaponClass)
	{
		TestWeapon = GetWorld()->SpawnActor<AWeaponActor>(TestWeaponClass);
		if (TestWeapon)
		{
			TestWeapon->AttachToComponent(
				GetMesh(), 
				FAttachmentTransformRules::KeepRelativeTransform, 
				TEXT("hand_rSocket"));
			TestWeapon->SetOwnerPlayer(this);

			CurrentWeapon = TestWeapon;	// 임시
		}
	}
}
