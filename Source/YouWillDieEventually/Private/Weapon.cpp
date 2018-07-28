// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "YouWillDieEventually.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon::AWeapon()
{
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	WeaponBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponBase"));
	WeaponBase->SetCollisionProfileName("NoCollision");
	WeaponBase->SetupAttachment(RootComp);

	WeaponBarrel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponBarrel"));
	WeaponBarrel->SetCollisionProfileName("NoCollision");
	WeaponBarrel->SetupAttachment(RootComp);

	WeaponHammer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponHammer"));
	WeaponHammer->SetCollisionProfileName("NoCollision");
	WeaponHammer->SetupAttachment(RootComp);

	WeaponMagazine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMagazine"));
	WeaponMagazine->SetCollisionProfileName("NoCollision");
	WeaponMagazine->SetupAttachment(RootComp);

	WeaponSlide = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponSlide"));
	WeaponSlide->SetCollisionProfileName("NoCollision");
	WeaponSlide->SetupAttachment(RootComp);

	WeaponTrigger = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponTrigger"));
	WeaponTrigger->SetCollisionProfileName("NoCollision");
	WeaponTrigger->SetupAttachment(RootComp);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(RootComp);

	WeaponRange = 10000;
	WeaponBaseDamage = 25;
	WeaponCriticalMultiplier = 4;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::Shoot() {
	FVector StartLocation = MuzzleLocation->GetComponentLocation();
	FVector EndLocation = StartLocation + (MuzzleLocation->GetComponentRotation().Vector() * WeaponRange);
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.bTraceComplex = true;
	CollisionParams.bReturnPhysicalMaterial = true;

	bool bLineTraceSuccess = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, WEAPONCHANNEL, CollisionParams);
	
	if (bLineTraceSuccess) {
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
		float DamageToApply = WeaponBaseDamage;
		
		if (SurfaceType == FLESH_VULNERABLE) {
			DamageToApply *= WeaponCriticalMultiplier;
		}
		
		UGameplayStatics::ApplyPointDamage(HitResult.GetActor(), DamageToApply, HitResult.ImpactNormal, HitResult, GetInstigatorController(), this, nullptr);
		PlayImpactEffect(SurfaceType, HitResult.ImpactPoint);
	}
	
	PlayFireEffects(StartLocation);
	PlayFireSounds(StartLocation);

}

void AWeapon::PlayImpactEffect(EPhysicalSurface SurfaceType, FVector Location) {
	UParticleSystem* HitEffectToPlay;
	
	switch (SurfaceType) {
		case FLESH_NORMAL:
		case FLESH_VULNERABLE:
			HitEffectToPlay = FleshHitEffect;
			break;
		default:
			HitEffectToPlay = DefaultHitEffect;
			break;
	}

	UGameplayStatics::SpawnEmitterAtLocation(this, HitEffectToPlay, Location);
}

void AWeapon::PlayFireEffects(FVector Location) {
	if (FireEffect) {
		UGameplayStatics::SpawnEmitterAttached(FireEffect, MuzzleLocation);
	}
}

void AWeapon::PlayFireSounds(FVector Location) {
	if (FireSounds.Num() > 0) {
		int32 RandomInt = FMath::RandRange(0, FireSounds.Num() - 1);
		UGameplayStatics::PlaySoundAtLocation(this, FireSounds[RandomInt], Location);
	}
}