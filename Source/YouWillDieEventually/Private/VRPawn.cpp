// Fill out your copyright notice in the Description page of Project Settings.

#include "VRPawn.h"
#include "Weapon.h"
#include "FlashLight.h"
#include "HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "MotionControllerComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"


// Sets default values
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	SetRootComponent(VRRoot);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(VRRoot);

	ControllerRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("ControllerRight"));
	ControllerRight->SetTrackingSource(EControllerHand::Right);
	ControllerRight->SetupAttachment(VRRoot);

	ControllerLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("ControllerLeft"));
	ControllerLeft->SetTrackingSource(EControllerHand::Left);
	ControllerLeft->SetupAttachment(VRRoot);

	ControllerRightMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightHandMesh"));
	ControllerRightMesh->SetCollisionProfileName("NoCollision");
	ControllerRightMesh->SetupAttachment(ControllerRight);

	ControllerLeftMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftHandMesh"));
	ControllerLeftMesh->SetCollisionProfileName("NoCollision");
	ControllerLeftMesh->SetupAttachment(ControllerLeft);

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &AVRPawn::HandleOnHealthChanged);

	AmmoCounterWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("AmmoCounterWidgetComp"));
	AmmoCounterWidgetComp->SetupAttachment(ControllerRightMesh);

	HealthCounterWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthCounterWidgetComp"));
	HealthCounterWidgetComp->SetupAttachment(ControllerRightMesh);

	GripSocketName = "GripSocket";

	DefaultAmmoCount = 15;
}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();
	
	bDied = false;
	CurrentAmmoCount = DefaultAmmoCount;
	UpdateAmmoCountWidget(CurrentAmmoCount);

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	if (CharacterWeaponClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CharacterWeapon = GetWorld()->SpawnActor<AWeapon>(CharacterWeaponClass, SpawnParams);
		CharacterWeapon->AttachToComponent(ControllerRightMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GripSocketName);
	}

	if (CharacterFlashLightClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CharacterFlashLight = GetWorld()->SpawnActor<AFlashLight>(CharacterFlashLightClass, SpawnParams);
		CharacterFlashLight->AttachToComponent(ControllerLeftMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GripSocketName);
	}
}

// Called every frame
void AVRPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Shoot", IE_Released, this, &AVRPawn::Shoot);
}

void AVRPawn::Shoot() {
	if (CharacterWeapon && CurrentAmmoCount > 0) {
		CharacterWeapon->Shoot();
		
		--CurrentAmmoCount;
		UpdateAmmoCountWidget(CurrentAmmoCount);
	}

	// TODO: play empty mag sound when out of ammo.
}

void AVRPawn::HandleOnHealthChanged(float CurrentHealth, float DamageApplied) {

	if (bDied) { return; }

	if (CurrentHealth <= 0.f) {
		bDied = true;

		// TODO: Figure out how VR Player will die.

		// Remove the weapon
		if (CharacterWeapon) {
			CharacterWeapon->Destroy();
		}
	}

	int32 Health = (int32)CurrentHealth;
	UpdateHealthCountWidget(Health);
}