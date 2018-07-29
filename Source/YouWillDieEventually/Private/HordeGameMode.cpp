// Fill out your copyright notice in the Description page of Project Settings.

#include "HordeGameMode.h"
#include "Zombie.h"
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
	UE_LOG(LogTemp, Warning, TEXT("Preparing for next wave!"));
}

void AHordeGameMode::StartWave() {
	// TODO: Play wave starting sound
	UE_LOG(LogTemp, Warning, TEXT("Wave started!"));

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
	}

	// If there's enemies to spawn left
	// Decrement one from enemies to spawn
	EnemiesToSpawn--;
	
	// Spawn a new enemy
	SpawnEnemy();
	UE_LOG(LogTemp, Warning, TEXT("Enemy spawned!"));
}

void AHordeGameMode::EndWave() {
	GetWorldTimerManager().ClearTimer(TimerHandle_PollWaveState);

	PrepareForNextWave();
	UE_LOG(LogTemp, Warning, TEXT("Wave ended!"));
}

void AHordeGameMode::PollWaveState() {
	UE_LOG(LogTemp, Warning, TEXT("Polling wave state!"));
	// Check if there's any enemy left
	TArray<AActor*> ZombiesArray;
	UGameplayStatics::GetAllActorsOfClass(this, AZombie::StaticClass(), ZombiesArray);
	
	// If all zombies are dead
	if (ZombiesArray.Num() <= 0) {
		EndWave();
	}
}