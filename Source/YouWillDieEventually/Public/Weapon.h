// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class YOUWILLDIEEVENTUALLY_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	void Shoot();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* WeaponBase;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* WeaponBarrel;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* WeaponHammer;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* WeaponMagazine;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* WeaponSlide;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UStaticMeshComponent* WeaponTrigger;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USceneComponent* MuzzleLocation;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float WeaponRange;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float WeaponBaseDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float WeaponCriticalMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TArray<class USoundBase*> FireSounds;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	class UParticleSystem* FireEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	class UParticleSystem* FleshHitEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	class UParticleSystem* DefaultHitEffect;

protected:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void PlayFireEffects(FVector Location);

	void PlayFireSounds(FVector Location);

	void PlayImpactEffect(EPhysicalSurface SurfaceType, FVector Location);
};