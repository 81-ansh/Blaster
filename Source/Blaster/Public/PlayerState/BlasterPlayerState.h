// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlasterTypes/Team.h"
#include "GameFramework/PlayerState.h"
#include "BlasterPlayerState.generated.h"

class ABlasterPlayerController;
class ABlasterCharacter;

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	/*
	 * Replication Notifies
	 */
	
	virtual void OnRep_Score() override;
	
	UFUNCTION()
	virtual void OnRep_Death();
	
	void AddToScore(float ScoreAmount);
	void AddToDeath(int32 DeathAmount);
	
	FORCEINLINE ETeam GetTeam() const { return Team; }
	FORCEINLINE void SetTeam(ETeam TeamToSet) { Team = TeamToSet; }
	
private:
	
	UPROPERTY()
	ABlasterCharacter* Character;
	
	UPROPERTY()
	ABlasterPlayerController* Controller;
	
	UPROPERTY(ReplicatedUsing = OnRep_Death)
	int32 Deaths;
	
	UPROPERTY(Replicated)
	ETeam Team = ETeam::ET_NoTeam;
	
};
