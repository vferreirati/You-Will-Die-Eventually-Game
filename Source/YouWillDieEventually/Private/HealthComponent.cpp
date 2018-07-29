// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"


// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	MaxHealth = 100;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	// subscribe to owner take any damage event
	if (AActor* Owner = GetOwner()) {
		Owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleOwnerTakeDamage);
	}
}

void UHealthComponent::HandleOwnerTakeDamage(AActor* DamagedActor, float Damage,
	const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) {
	
	if (Damage <= 0) { return; }
	CurrentHealth -= Damage;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);

	// broadcast that the health value changed
	OnHealthChanged.Broadcast(CurrentHealth, Damage);
}

void UHealthComponent::Heal(float AmountToHeal) {
	CurrentHealth += AmountToHeal;

	CurrentHealth = FMath::Clamp(CurrentHealth, 0.f, MaxHealth);

	OnHealthChanged.Broadcast(CurrentHealth, 0.f);
}