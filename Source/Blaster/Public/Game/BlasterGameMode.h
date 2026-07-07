// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BlasterGameMode.generated.h"

namespace MatchState
{
	extern BLASTER_API const FName Cooldown;	// Match is finished, Display winner and begin cooldown timer.
}

class ABlasterPlayerController;
class ABlasterCharacter;

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	
	ABlasterGameMode();
	virtual void Tick(float DeltaTime);
	
	virtual void PlayerEliminated(ABlasterCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController, ABlasterPlayerController* AttackerController);
	virtual void RequestRespawn(ACharacter* ElimmedCharacter, AController* ElimmedController);
	
	UPROPERTY(EditDefaultsOnly)
	float WarmupTime = 10.f;			// Time before match
	
	UPROPERTY(EditDefaultsOnly)
	float MatchTime = 120.f;
	
	UPROPERTY(EditDefaultsOnly)
	float CooldownTime = 10.f;			// Time after match
	
	float LevelStartingTime = 0.f;
	
protected:
	
	virtual void BeginPlay() override;
	virtual void OnMatchStateSet() override;
	
private:
	
	float CountdownTime = 0.f;
	
};
