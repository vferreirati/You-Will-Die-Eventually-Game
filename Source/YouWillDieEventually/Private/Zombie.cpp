// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"
#include "HordeGameMode.h"
#include "HealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "Gameframework/PawnMovementComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "YouWillDieEventually.h"

// Sets default values
AZombie::AZombie()
{
	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(WEAPONCHANNEL, ECollisionResponse::ECR_Ignore);
	BaseDamage = 10.f;
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComp->OnHealthChanged.AddDynamic(this, &AZombie::HandleOnHealthChanged);	
	bDied = false;
	bIsChasing = false;
	bIsAttacking = false;
	RandomIndex = FMath::RandRange(0, 2);
	PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
}

void AZombie::HandleOnHealthChanged(float CurrentHealth, float AppliedDamage) {
	if (bDied) { return; }
	
	if(CurrentHealth <= 0.f)  {
		// Died
		bDied = true;

		GetMovementComponent()->StopMovementImmediately();

		// Stop causing damage to player
		EndAttack();

		// Disable collision of his CapsuleComp
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// Detach from controller
		DetachFromControllerPendingDestroy();

		// Timer to get removed from map
		SetLifeSpan(5.f);

		NotifyDeath();
	}
}

void AZombie::BeginAttack() {
	if (bIsAttacking) { return; }

	bIsAttacking = true;

	GetWorldTimerManager().SetTimer(TimerHandle_AttackInterval, this, &AZombie::Attack, 1.f, true, 0.f);
}

void AZombie::EndAttack() {
	if (!bIsAttacking) { return; }

	bIsAttacking = false;
	GetWorldTimerManager().ClearTimer(TimerHandle_AttackInterval);
}

void AZombie::Attack() {
	UGameplayStatics::ApplyDamage(PlayerPawn, BaseDamage, GetInstigatorController(), this, nullptr);
}

void AZombie::NotifyDeath() {
	AHordeGameMode* GM = Cast<AHordeGameMode>(GetWorld()->GetAuthGameMode());
	if (GM) {
		GM->OnZombieDeath();
	}
}