// Fill out your copyright notice in the Description page of Project Settings.

#include "VRPawn.h"
#include "Weapon.h"
#include "HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
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

	GripSocketName = "GripSocket";
}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();
	
	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	if (CharacterWeaponClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		CharacterWeapon = GetWorld()->SpawnActor<AWeapon>(CharacterWeaponClass, SpawnParams);
		CharacterWeapon->AttachToComponent(ControllerRightMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, GripSocketName);
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
	if (CharacterWeapon) {
		CharacterWeapon->Shoot();
	}
}

void AVRPawn::HandleOnHealthChanged(float CurrentHealth, float DamageApplied) {
	if (CurrentHealth <= 0.f) {
		// TODO: Figure out how VR Player will die.

		// Remove the weapon
		if (CharacterWeapon) {
			CharacterWeapon->Destroy();
		}
	}
}