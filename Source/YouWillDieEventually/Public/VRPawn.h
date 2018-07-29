// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VRPawn.generated.h"

UCLASS()
class YOUWILLDIEEVENTUALLY_API AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:	

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UMotionControllerComponent* ControllerRight;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UMotionControllerComponent* ControllerLeft;
	
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USkeletalMeshComponent* ControllerRightMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USkeletalMeshComponent* ControllerLeftMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UHealthComponent* HealthComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWidgetComponent* AmmoCounterWidgetComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UWidgetComponent* HealthCounterWidgetComp;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<class AWeapon> CharacterWeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	TSubclassOf<class AFlashLight> CharacterFlashLightClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Setup")
	FName GripSocketName;

	// Amounts of bullets the player will start with
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 DefaultAmmoCount;

	int32 CurrentAmmoCount;

	class AWeapon* CharacterWeapon;

	class AFlashLight* CharacterFlashLight;

	bool bDied;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void Shoot();

	UFUNCTION()
	void HandleOnHealthChanged(float CurrentHealth, float AppliedDamage);

	UFUNCTION(BlueprintImplementableEvent, Category = "VRPawn")
	void UpdateAmmoCountWidget(int32 CurrentAmmount);

	UFUNCTION(BlueprintImplementableEvent, Category = "VRPawn")
	void UpdateHealthCountWidget(int32 CurrentHealth);
};
