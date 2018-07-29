// Fill out your copyright notice in the Description page of Project Settings.

#include "HordeGameMode.h"
#include "Zombie.h"
#include "VRPawn.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

AHordeGameMode::AHordeGameMode() {
	TimeBetweenWaves = 10.f;
	TimeBetweenSpawns = 1.f;
	NumberOfEnemiesInFirstWave = 10;
	EnemiesIncrementValue = 7;
	CurrentWaveNumber = 0;
}

void AHordeGameMode::StartPlay() {
	Super::StartPlay();

	PrepareForNextWave();
}

void AHordeGameMode::PrepareForNextWave() {
	// Start wave countdown
	GetWorldTimerManager().SetTimer(TimerHandle_WaveStartingCountdown, this, &AHordeGameMode::StartWave, TimeBetweenWaves);
}

void AHordeGameMode::StartWave() {
	// TODO: Play wave starting sound

	// Update Wave Number
	CurrentWaveNumber++;
	UpdateCurrentWaveNumber(CurrentWaveNumber);

	// Update Number of Enemies To Spawn
	EnemiesToSpawn = NumberOfEnemiesInFirstWave + CurrentWaveNumber * EnemiesIncrementValue;

	// Start spawning enemies
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnEnemyDelay, this, &AHordeGameMode::SpawnEnemyTick, TimeBetweenSpawns, true, 0.f);
}

void AHordeGameMode::SpawnEnemyTick() {
	// If out of enemies to spawn
	if (EnemiesToSpawn <= 0) {
		// Stop ticking SpawnEnemy
		GetWorldTimerManager().ClearTimer(TimerHandle_SpawnEnemyDelay);
		
		// Start Polling Wave State
		GetWorldTimerManager().SetTimer(TimerHandle_PollWaveState, this, &AHordeGameMode::PollWaveState, 1.f, true);

		return;
	}

	// If there's enemies to spawn left
	// Decrement one from enemies to spawn
	EnemiesToSpawn--;
	
	// Spawn a new enemy
	SpawnEnemy();
}

void AHordeGameMode::EndWave() {
	GetWorldTimerManager().ClearTimer(TimerHandle_PollWaveState);

	SupplyPlayer();
	PrepareForNextWave();
}

void AHordeGameMode::PollWaveState() {
	// Check if there's any enemy left
	TArray<AActor*> ZombiesArray;
	UGameplayStatics::GetAllActorsOfClass(this, AZombie::StaticClass(), ZombiesArray);
	
	// If all zombies are dead
	if (ZombiesArray.Num() <= 0) {
		EndWave();
	}
}

void AHordeGameMode::SupplyPlayer() {
	AVRPawn* PlayerPawn = Cast<AVRPawn>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (PlayerPawn) {
		int32 AmmoAmount = NumberOfEnemiesInFirstWave + CurrentWaveNumber * EnemiesIncrementValue * 1.5;	// TODO: Validate this
		PlayerPawn->ReceiveSupplies(AmmoAmount, 20.f);
	}
}