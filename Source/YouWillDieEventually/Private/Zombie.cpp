// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Gameframework/PawnMovementComponent.h"
#include "YouWillDieEventually.h"

// Sets default values
AZombie::AZombie()
{
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(WEAPONCHANNEL, ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComp->OnHealthChanged.AddDynamic(this, &AZombie::HandleOnHealthChanged);	
	bDied = false;
	bIsChasing = false;
	RandomIndex = FMath::RandRange(0, 2);
}

void AZombie::HandleOnHealthChanged(float CurrentHealth, float AppliedDamage) {
	if (bDied) { return; }
	
	if(CurrentHealth <= 0.f)  {
		// Died
		bDied = true;

		GetMovementComponent()->StopMovementImmediately();

		// Disable collision of his CapsuleComp
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// Detach from controller
		DetachFromControllerPendingDestroy();

		// Timer to get removed from map
		SetLifeSpan(5.f);
	}
}