// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Zombie.generated.h"

UCLASS()
class YOUWILLDIEEVENTUALLY_API AZombie : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AZombie();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UHealthComponent* HealthComp;

	UPROPERTY(BlueprintReadWrite, Category = "Zombie")
	bool bIsChasing;

	UPROPERTY(BlueprintReadOnly, Category = "Zombie")
	bool bDied;

	UPROPERTY(BlueprintReadOnly, Category = "Zombie")
	bool bIsAttacking;

	UPROPERTY(BlueprintReadOnly, Category = "Zombie")
	int32 RandomIndex;

	UPROPERTY(EditDefaultsOnly, Category = "Zombie")
	float BaseDamage;

	FTimerHandle TimerHandle_AttackInterval;

	class APawn* PlayerPawn;
	

protected:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Zombie")
	void BeginAttack();

	void EndAttack();

	UFUNCTION()
	void Attack();
	
	UFUNCTION()
	void HandleOnHealthChanged(float CurrentHealth, float AppliedDamage);

	// Tells the gamemode a zombie died
	void NotifyDeath();
};
