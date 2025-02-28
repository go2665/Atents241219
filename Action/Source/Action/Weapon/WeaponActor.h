// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

UCLASS()
class ACTION_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponActor();

	// 무기의 활성화 여부를 설정한다.
	void WeaponActivate(bool bActivate);

	// 무기의 충돌 활성화 여부를 설정한다.
	void SetCollisionActivate(bool bActivate);

	inline float GetDamage() const { return Damage; }

	inline void SetOwnerPlayer(class AActionPlayerCharacter* InOwner) { OwnerPlayer = InOwner; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnWeaponBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);
public:	

	virtual void PostInitializeComponents() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class UCapsuleComponent* WeaponCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weapon")
	float Damage = 10.0f;

private:
	UPROPERTY()
	AActionPlayerCharacter* OwnerPlayer = nullptr;
};
