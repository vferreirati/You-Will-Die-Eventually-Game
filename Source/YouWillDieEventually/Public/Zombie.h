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

	bool bDied;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UHealthComponent* HealthComp;

protected:	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void HandleOnHealthChanged(float CurrentHealth, float AppliedDamage);
};
