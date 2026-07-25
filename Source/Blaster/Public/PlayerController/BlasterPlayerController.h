// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

class UReturnToMainMenu;
class UCharacterOverlay;
class ABlasterGameMode;
class ABlasterHUD;
class UInputAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHighPingDelegate, bool, bPingTooHigh);

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDShield(float Shield, float MaxShield);
	void SetHUDScore(float Score);
	void SetHUDDeath(int32 Deaths);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 Ammo);
	void SetHUDMatchCountdown(float CountdownTime);
	void SetHUDAnnouncementCountdown(float CountdownTime);
	void SetHUDGrenade(int32 Grenade);
	virtual void OnPossess(APawn* InPawn) override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps( TArray<FLifetimeProperty>& OutLifetimeProps ) const override;
	
	virtual float GetServerTime();				// Synced with server world clock
	virtual void ReceivedPlayer() override;		// Sync with server as soon as possible
	void OnMatchStateSet(FName State);
	void HandleMatchStart();
	void HandleCooldown();
	
	float SingleTripTime = 0.f;
	
	FHighPingDelegate HighPingDelegate;
	
protected:
	
	virtual void BeginPlay() override;
	void SetHUDTime();
	
	virtual void SetupInputComponent() override;
	
	void ShowReturnToMainMenu();
	
	/*
	 * Sync time between client and server
	 */
	
	// Request the current server time, passing in the client's time when the request was sent
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);
	
	// Reports the current server time to the client in response to ServerRequestServerTime
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);
	
	float ClientServerDelta = 0.f;	// difference between client and server time
	
	UPROPERTY(EditAnywhere, Category = "Time")
	float TimeSyncFrequency = 5.f;		// Interval between the time sync
	
	float TimeSyncRunningTime = 0.f;	// Time passed since last sync
	
	void CheckTimeSync(float DeltaTime);
	
	UFUNCTION(Server, Reliable)
	void ServerCheckMatchState();
	
	UFUNCTION(Client, Reliable)
	void ClientJoinMidgame(FName InMatchState, float InWarmupTime, float InMatchTime, float InCooldownTime, float InLevelStartingTime);
	
	void HighPingWarning();
	void StopHighPingWarning();
	void CheckPing(float DeltaSeconds);
	
private:
	
	UPROPERTY()
	ABlasterHUD* BlasterHUD;
	
	UPROPERTY()
	ABlasterGameMode* BlasterGameMode;
	
	float LevelStartingTime = 0.f;
	float WarmupTime = 0.f;
	float MatchTime = 0.f;
	float CooldownTime = 0.f;
	uint32 CountdownInt = 0;
	
	UPROPERTY(ReplicatedUsing = OnRep_MatchState)
	FName MatchState;
	
	UFUNCTION()
	void OnRep_MatchState();
	
	UPROPERTY()
	UCharacterOverlay* CharacterOverlay;

	float HUDCarriedAmmo;
	bool bInitializeCarriedAmmo = false;

	float HUDWeaponAmmo;
	bool bInitializeWeaponAmmo = false;

	void PollInit();
	
	float HighPingRunningTime = 0.f;	// Time since the last warning shown
	
	UPROPERTY(EditAnywhere)
	float HighPingDuration = 5.f;		// Animation Duration
	
	float PingAnimationRunningTime = 0.f;
	
	UPROPERTY(EditAnywhere)
	float CheckPingFrequency = 20.f;	// Time interval in which ping will be checked
	
	UPROPERTY(EditAnywhere)
	float HighPingThreshold = 50.f;		// The limit after which will be considered HighPing
	
	UFUNCTION(Server, Reliable)
	void ServerReportPingStatus(bool bHighPing);	// Checking if ping is too high
	
	/*
	 * Return To Main Menu
	 */
	
	UPROPERTY(EditAnywhere, Category = "HUD")
	TSubclassOf<UUserWidget> ReturnToMainMenuWidget;	// Class that will be created
	
	UPROPERTY()
	TObjectPtr<UReturnToMainMenu> ReturnToMainMenu;		// Storing the Widget after creating
	
	bool bReturnToMainMenuOpen = false;					// Is menu open or not
	
	UPROPERTY(EditAnywhere, Category = "HUD")
	TObjectPtr<UInputAction> QuitAction;
	
};
