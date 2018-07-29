// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HordeGameMode.generated.h"

/**
 * 
 */
UCLASS()
class YOUWILLDIEEVENTUALLY_API AHordeGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AHordeGameMode();

	virtual void StartPlay() override;

protected:
	// Amount of seconds of preparation between in game waves.
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float TimeBetweenWaves;

	// Delay between enemy spawns. Eg: 2 per second = 0.5
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	float TimeBetweenSpawns;

	// Amount of enemies spawned on the very first wave.
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 NumberOfEnemiesInFirstWave;

	// Amount to increment the amount of enemies per wave
	UPROPERTY(EditDefaultsOnly, Category = "Setup")
	int32 EnemiesIncrementValue;

	int32 CurrentWaveNumber;

	int32 EnemiesToSpawn;

	FTimerHandle TimerHandle_WaveStartingCountdown;

	FTimerHandle TimerHandle_SpawnEnemyDelay;

	FTimerHandle TimerHandle_PollWaveState;

protected:

	// Updates properties for next wave
	void PrepareForNextWave();
	
	// Starts spawning enemies in the world.
	void StartWave();

	// Stops current wave and Begins preparation for next.
	void EndWave();

	// Checks if all enemies are killed
	void PollWaveState();
	
	void SpawnEnemyTick();

	// Spawns a single enemy in the world
	UFUNCTION(BlueprintImplementableEvent, Category = "HordeGameMode")
	void SpawnEnemy();

	// Updates the Score board with current wave number
	UFUNCTION(BlueprintImplementableEvent, Category = "HordeGameMode")
	void UpdateCurrentWaveNumber(int32 WaveNumber);

	void SupplyPlayer();
};
