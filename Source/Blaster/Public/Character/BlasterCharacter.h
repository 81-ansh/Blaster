// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "NiagaraComponent.h"
#include "BlasterComponents/CombatComponent.h"
#include "BlasterTypes/CombatState.h"
#include "BlasterTypes/TurningInPlace.h"
#include "Camera/CameraComponent.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "Interfaces/InteractWithCrosshairInterface.h"
#include "BlasterTypes/Team.h"
#include "BlasterCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeftGame);

class UBoxComponent;
class AWeapon;
class UAnimMontage;
class ABlasterPlayerState;
class ABlasterPlayerController;
class USpringArmComponent;
class UWidgetComponent;
class UBuffComponent;
class ULagCompensationComponent;

UCLASS()
class BLASTER_API ABlasterCharacter : public ACharacter, public IInteractWithCrosshairInterface
{
	GENERATED_BODY()

public:
	
	ABlasterCharacter();
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	void PlayFireMontage(bool bAiming);
	void PlayReloadMontage();
	void CancelReloadMontage();
	void PlayThrowGrenadeMontage();
	void PlaySwapMontage();
	virtual void OnRep_ReplicatedMovement() override;
	void Elim(bool bPlayerLeftGame);
	virtual void Destroyed() override;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim(bool bPlayerLeftGame);
	
	UPROPERTY(Replicated)
	bool bDisableGameplay = false;
	
	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);
	
	void UpdateHUDHealth();
	void UpdateHUDShield();
	void UpdateHUDAmmo();
	
	void SpawnDefaultWeapon();
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMapping;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> EquipAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> CrouchAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ReloadAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> AimAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> FireAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> GrenadeThrowAction;
	
	UPROPERTY()
	ABlasterPlayerState* BlasterPlayerState;
	
	UPROPERTY()
	TMap<FName, UBoxComponent*> HitCollisionBoxes;
	
	bool bFinishedSwapping = false;
	
	UFUNCTION(Server, Reliable)
	void ServerLeaveGame();
	
	FOnLeftGame OnLeftGame;
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastGainedTheLead();
	
	UFUNCTION(NetMulticast, Reliable)
	void MulticastLostTheLead();
	
	void SetTeamColor(ETeam Team);
	
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped();
	bool IsAiming();
	AWeapon* GetEquippedWeapon();
	FVector GetHitTarget() const;
	ECombatState GetCombatState() const;
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	FORCEINLINE bool IsElimmed() const { return bElimmed; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE void SetHealth(float Amount) { Health = Amount; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetShield() const { return Shield; }
	FORCEINLINE void SetShield(float Amount) { Shield = Amount; }
	FORCEINLINE float GetMaxShield() const { return MaxShield; }
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	FORCEINLINE UBuffComponent* GetBuff() const { return Buff; }
	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }
	FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; }
	FORCEINLINE UStaticMeshComponent* GetAttachedGrenade() const { return AttachedGrenade; }
	FORCEINLINE ULagCompensationComponent* GetLagCompensation() const { return LagCompensation; }
	bool IsLocallyReloading();

protected:
	
	virtual void BeginPlay() override;
	
	void Move(const FInputActionInstance& Instance);
	void Look(const FInputActionInstance& Instance);
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void ReloadButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void FireButtonPressed();
	void FireButtonReleased();
	void GrenadeButtonPressed();
	void CalculateAO_Pitch();
	void AimOffset(float DeltaTime);
	void SimProxiesTurn();
	virtual void Jump() override;
	void PlayHitReactMontage();
	void PlayElimMontage();
	void DropOrDestroyWeapon(AWeapon* Weapon);
	
	// Poll for any relevant class and initialize HUD
	void PollInit();
	void RotateInPlace(float DeltaTime);
	
	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCauser);
	
	/*
	 * Hit Boxes used for server side rewind
	 */
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> head;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> pelvis;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> spine_02;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> spine_03;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> upperarm_l;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> upperarm_r;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> lowerarm_l;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> lowerarm_r;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> hand_l;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> hand_r;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> backpack;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> blanket;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> thigh_l;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> thigh_r;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> calf_l;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> calf_r;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> foot_l;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> foot_r;

private:
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* OverheadWidget;
	
	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	AWeapon* OverlappingWeapon;
	
	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);
	
	/*
	 * Blaster Components
	 */
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCombatComponent* Combat;
	
	UPROPERTY(VisibleAnywhere)
	UBuffComponent* Buff;
	
	UPROPERTY(VisibleAnywhere)
	ULagCompensationComponent* LagCompensation;
	
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();
	
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;
	
	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);
	
	/*
	 * Animation Montage
	 */
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> FireWeaponMontage;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> ReloadMontage;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> ElimMontage;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> ThrowGrenadeMontage;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<UAnimMontage> SwapMontage;

	void HideCharacterIfCameraClose();
	
	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;
	
	bool bRotateRootBone;
	float TurnThreshold = 0.5f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	float CalculateSpeed();
	
	/*
	 * Player Health
	 */
	
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxHealth = 100.f;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Health, Category = "Player Stats")
	float Health = 100.f;
	
	UFUNCTION()
	void OnRep_Health(float LastHealth);
	
	/*
	 * Player Shield
	 */
	
	UPROPERTY(EditAnywhere, Category = "Player Stats")
	float MaxShield = 100.f;
	
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_Shield, Category = "Player Stats")
	float Shield = 0.f;
	
	UFUNCTION()
	void OnRep_Shield(float LastShield);
	
	UPROPERTY()
	ABlasterPlayerController* BlasterPlayerController;
	
	bool bElimmed = false;
	
	FTimerHandle ElimTimer;
	
	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;
	
	void ElimTimerFinished();
	
	bool bLeftGame = false;
	
	/*
	 * Dissolve Effect
	 */
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UTimelineComponent> DissolveTimeline;
	
	FOnTimelineFloat DissolveTrack;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UCurveFloat> DissolveCurve;
	
	// Dynamic Instance that can be changed at runtime
	UPROPERTY(VisibleAnywhere, Category = "Elim")
	TObjectPtr<UMaterialInstanceDynamic> DynamicDissolveMaterialInstance;
	
	// Material Instance set on the Blueprint
	UPROPERTY(VisibleAnywhere, Category = "Elim")
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	
	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
	
	void StartDissolve();
	
	/*
	 * Team Colors
	 */
	
	UPROPERTY(EditAnywhere, Category = "Elim")
	TObjectPtr<UMaterialInstance> RedDissolveMatInst;
	
	UPROPERTY(EditAnywhere, Category = "Elim")
	TObjectPtr<UMaterialInstance> RedMaterial;
	
	UPROPERTY(EditAnywhere, Category = "Elim")
	TObjectPtr<UMaterialInstance> BlueDissolveMatInst;
	
	UPROPERTY(EditAnywhere, Category = "Elim")
	TObjectPtr<UMaterialInstance> BlueMaterial;
	
	UPROPERTY(EditAnywhere, Category = "Elim")
	TObjectPtr<UMaterialInstance> OriginalMaterial;
	
	/*
	 * Elim Effects
	 */
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ElimBotEffect;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> ElimBotComponent;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ElimBotSound;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> CrownSystem;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> CrownComponent;
	
	/*
	 * Grenade
	 */
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> AttachedGrenade;
	
	/*
	 * Default Weapon
	 */
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> DefaultWeaponClass;
	
};
